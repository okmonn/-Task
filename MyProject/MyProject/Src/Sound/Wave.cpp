#include "Wave.h"
#include "../XAudio2/XAudio2.h"
#include "../XAudio2/VoiceCallback.h"
#include "SoundFunc.h"
#include "../Func/Func.h"
#include <ks.h>
#include <ksmedia.h>
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
	audio(XAudio2::Get()), back(std::make_unique<VoiceCallback>()), voice(nullptr), file(nullptr), channel(0), sample(0), bit(0), 
	start(0), index(0)
{
	data.clear();

	func = &Wave::Loading;
}

// デストラクタ
Wave::~Wave()
{
	Destroy(voice);
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
	fmt.Format.nAvgBytesPerSec = sample * channel * func::Byte(bit);
	fmt.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);

	fmt.Samples.wValidBitsPerSample = sample;
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
		return;
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

	//ソースボイス生成
	if (FAILED(Create()))
	{
		Close(file);
		return -1;
	}

	//波形データの開始位置取得
	start = (riff.size + sizeof(riff.id) + sizeof(riff.size)) - data.size;

	return 0;
}

// 波形をすべて読み込み終わってない場合の処理
void Wave::Loading(void)
{
	XAUDIO2_VOICE_STATE st = {};

	while (true)
	{
		voice->GetState(&st);
		if (st.BuffersQueued < BUFFER_NUM)
		{
			//配列のメモリ確保
			data[index].resize(fmt.byte / (func::Byte(fmt.bit) * fmt.channel));

			int flag = 0;
			switch (channel)
			{
			case 1:
				flag = sound::LoadMono(data[index], bit, file);
				break;
			case 2:
				flag = sound::LoadStereo(data[index], bit, file);
				break;
			default:
				break;
			}

			//波形のファイル読み込みが失敗
			if (flag != 0)
			{
				threadFlag = false;
				Close(file);
				break;
			}

			//sound::Delay(stream[index], fmt.Format.nSamplesPerSec, bit, 0.3f, 0.05f, 10);

			//バッファに追加
			XAUDIO2_BUFFER buf = {};
			buf.AudioBytes = sizeof(float) * stream[index].size();
			buf.pAudioData = (BYTE*)(&stream[index][0]);

			auto hr = voice->SubmitSourceBuffer(&buf);
			if (FAILED(hr))
			{
				OutputDebugString(_T("\nバッファの追加：失敗\n"));
				threadFlag = false;
				Close(file);
				break;
			}

			if (feof(file) != 0)
			{
				//ループ
				if (loop == true)
				{
					fseek(file, startPos, SEEK_SET);
				}
				else
				{
					//バッファが空になったとき
					if (st.BuffersQueued <= 0)
					{
						Stop();
						fseek(file, startPos, SEEK_SET);
					}
				}
			}

			//配列番号の更新
			index = (index >= stream.size() - 1) ? 0 : 1;
		}
	}
}

// 波形をすべて読み込み終わっている場合の処理
void Wave::Loaded(void)
{
}

