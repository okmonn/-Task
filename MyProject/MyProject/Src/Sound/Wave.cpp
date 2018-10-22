#include "Wave.h"
#include "../XAudio2/XAudio2.h"
#include "../XAudio2/VoiceCallback.h"
#include "SoundFunc.h"
#include "../Func/Func.h"
#include <ks.h>
#include <ksmedia.h>
#include <mutex>
#include <tchar.h>

// 解放マクロ
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }
#define Close(X) { if((X) != nullptr) fclose((X)); (X) = nullptr; }

// バッファの最大数
#define BUFFER_NUM 2

// スピーカー設定用配列
const DWORD spk[] = {
	KSAUDIO_SPEAKER_MONO,
	KSAUDIO_SPEAKER_STEREO,
	KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
	KSAUDIO_SPEAKER_QUAD,
	0,
	KSAUDIO_SPEAKER_5POINT1,
	0,
	KSAUDIO_SPEAKER_7POINT1_SURROUND
};

// コンストラクタ
Wave::Wave() : 
	audio(XAudio2::Get()), back(std::make_unique<VoiceCallback>()), voice(nullptr), file(nullptr), channel(0), sample(0), bit(0), byte(0),
	index(0), loop(false), play(false), threadFlag(true)
{
	data.clear();
}

// コンストラクタ
Wave::Wave(const std::string & fileName) :
	audio(XAudio2::Get()), back(std::make_unique<VoiceCallback>()), voice(nullptr), file(nullptr), channel(0), sample(0), bit(0), byte(0),
	index(0), play(false), threadFlag(true)
{
	data.clear();

	Load(fileName);
}

// デストラクタ
Wave::~Wave()
{
	Delete();
}

// 代入
void Wave::operator=(const Wave & w)
{
	if (voice != nullptr)
	{
		return;
	}

	channel = w.channel;
	sample  = sample;
	bit     = bit;
	byte    = byte;
	data    = w.data;

	Create();
	if (th.joinable() == false)
	{
		th = std::thread(&Wave::Stream, this);
	}
}

// ソースボイスの生成
long Wave::Create(void)
{
	//フォーマット設定用構造体
	WAVEFORMATEXTENSIBLE fmt = {};
	fmt.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;
	fmt.Format.nChannels       = channel;
	fmt.Format.nSamplesPerSec  = sample;
	fmt.Format.wBitsPerSample  = func::Bit(sizeof(float));
	fmt.Format.nBlockAlign     = sizeof(float) * channel;
	fmt.Format.nAvgBytesPerSec = sample * fmt.Format.nBlockAlign;
	fmt.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);

	fmt.Samples.wValidBitsPerSample = fmt.Format.wBitsPerSample;
	fmt.dwChannelMask               = spk[channel - 1];
	fmt.SubFormat                   = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

	auto hr = audio.GetAudio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&fmt), 0, 1.0f, back.get());
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nソースボイスの生成：失敗\n"));
	}

	return hr;
}

// 読み込み
int Wave::Load(const std::string & fileName)
{
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nファイルの参照：失敗\n"));
		return -1;
	}

	//チャンク宣言
	sound::RIFF riff = {};
	sound::FMT fmt   = {};
	sound::DATA data = {};

	//RIFF
	if (sound::LoadRIFF(riff, file) != 0)
	{
		return -1;
	}

	//FMT
	if (sound::LoadFMT(fmt, file) != 0)
	{
		return -1;
	}

	//DATA
	if (sound::LoadDATA(data, file) != 0)
	{
		return -1;
	}

	//必要なものの格納
	channel = fmt.channel;
	sample  = fmt.sample;
	bit     = fmt.bit;
	byte    = fmt.byte;

	//ソースボイス生成
	if (FAILED(Create()))
	{
		Close(file);
		return -1;
	}

	if (th.joinable() == false)
	{
		th = std::thread(&Wave::Stream, this);
	}

	return 0;
}

// 波形をすべて読み込み終わってない場合の処理
void Wave::Loading(void)
{
	//読み込み番号
	int read = 0;

	int flag = 0;

	XAUDIO2_VOICE_STATE st = {};

	while (std::feof(file) == 0 && threadFlag == true)
	{
		//配列のメモリ確保
		data[read].resize(byte / (func::Byte(bit) * channel));

		switch (channel)
		{
		case 1:
			flag = sound::LoadMono(data[read], bit, file);
			break;
		case 2:
			flag = sound::LoadStereo(data[read], bit, file);
			break;
		default:
			break;
		}

		//読み込みデータ番号を次に進める
		++read;

		voice->GetState(&st);
		if (st.BuffersQueued < BUFFER_NUM)
		{
			XAUDIO2_BUFFER buf = {};
			buf.AudioBytes = sizeof(float) * data[index].size();
			buf.pAudioData = (BYTE*)(&data[index][0]);

			auto hr = voice->SubmitSourceBuffer(&buf);
			if (FAILED(hr))
			{
				OutputDebugString(_T("\n波形データのバッファの追加：失敗\n"));
				threadFlag = false;
				break;
			}

			if (index + 1 >= data.size())
			{
				play = loop;
				index = 0;
			}
			else
			{
				++index;
			}
		}
	}

	Close(file);
}

// 波形をすべて読み込み終わっている場合の処理
void Wave::Loaded(void)
{
	XAUDIO2_VOICE_STATE st = {};

	while (threadFlag)
	{
		voice->GetState(&st);
		if (st.BuffersQueued < BUFFER_NUM)
		{
			//バッファに追加
			XAUDIO2_BUFFER buf = {};
			buf.AudioBytes = sizeof(float) * data[index].size();
			buf.pAudioData = (BYTE*)(&data[index][0]);

			auto hr = voice->SubmitSourceBuffer(&buf);
			if (FAILED(hr))
			{
				OutputDebugString(_T("\n波形データのバッファの追加：失敗\n"));
				threadFlag = false;
				break;
			}

			if (index + 1 >= data.size())
			{
				if (loop == true)
				{
					Stop();
				}
				index = 0;
			}
			else
			{
				++index;
			}
		}
	}

	st.BuffersQueued = 0;

}

// 非同期
void Wave::Stream(void)
{
	Loading();
	Loaded();
}

// 再生
long Wave::Play(const bool & loop)
{
	std::lock_guard<std::mutex>lock(std::mutex());

	this->loop = loop;

	auto hr = voice->Start();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n波形データの再生：失敗\n"));
	}

	return 0;
}

// 停止
long Wave::Stop(void)
{
	std::lock_guard<std::mutex>lock(std::mutex());

	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n波形データの停止：失敗\n"));
		return hr;
	}

	play = false;

	return hr;
}

// リセット
void Wave::Reset(void)
{
	std::lock_guard<std::mutex>lock(std::mutex());

	XAUDIO2_VOICE_STATE st = {};
	voice->GetState(&st);
	st.BuffersQueued = 0;
	index = 0;
}

// 波形データの削除
void Wave::Delete(void)
{
	Stop();

	threadFlag = false;
	if (th.joinable() == true)
	{
		th.join();
	}

	Destroy(voice);

	data.clear();

	index = 0;
}
