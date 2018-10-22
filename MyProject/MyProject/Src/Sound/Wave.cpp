#include "Wave.h"
#include "../XAudio2/XAudio2.h"
#include "../XAudio2/VoiceCallback.h"
#include "SoundFunc.h"
#include "../Func/Func.h"
#include <ks.h>
#include <ksmedia.h>
#include <tchar.h>

// ����}�N��
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }
#define Close(X) { if((X) != nullptr) fclose((X)); (X) = nullptr; }

// �o�b�t�@�̍ő吔
#define BUFFER_NUM 2

// �X�s�[�J�[�ݒ�p�z��
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

// �R���X�g���N�^
Wave::Wave() : 
	audio(XAudio2::Get()), back(std::make_unique<VoiceCallback>()), voice(nullptr), file(nullptr), channel(0), sample(0), bit(0), byte(0),
	index(0), arrival(false), threadFlag(true)
{
	data.clear();

	func = &Wave::Loading;
}

// �R���X�g���N�^
Wave::Wave(const std::string & fileName) :
	audio(XAudio2::Get()), back(std::make_unique<VoiceCallback>()), voice(nullptr), file(nullptr), channel(0), sample(0), bit(0), byte(0),
	index(0), arrival(false), threadFlag(true)
{
	data.clear();

	func = &Wave::Loading;

	Load(fileName);
}

// �f�X�g���N�^
Wave::~Wave()
{
	Delete();
}

// ���
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
	func = &Wave::Loaded;
	if (th.joinable() == false)
	{
		th = std::thread(&Wave::Stream, this);
	}
}

// �\�[�X�{�C�X�̐���
long Wave::Create(void)
{
	//�t�H�[�}�b�g�ݒ�p�\����
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
		OutputDebugString(_T("\n�\�[�X�{�C�X�̐����F���s\n"));
	}

	return hr;
}

// �ǂݍ���
int Wave::Load(const std::string & fileName)
{
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\n�t�@�C���̎Q�ƁF���s\n"));
		return -1;
	}

	//�`�����N�錾
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

	//�K�v�Ȃ��̂̊i�[
	channel = fmt.channel;
	sample  = fmt.sample;
	bit     = fmt.bit;
	byte    = fmt.byte;

	//�\�[�X�{�C�X����
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

// �g�`�����ׂēǂݍ��ݏI����ĂȂ��ꍇ�̏���
void Wave::Loading(void)
{
	//�ǂݍ��ݔԍ�
	int read = 0;

	int flag = 0;

	XAUDIO2_VOICE_STATE st = {};

	while (std::feof(file) == 0)
	{
		//�z��̃������m��
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

		//�ǂݍ��݃f�[�^�ԍ������ɐi�߂�
		++read;

		if (st.BuffersQueued < BUFFER_NUM)
		{
			XAUDIO2_BUFFER buf = {};
			buf.AudioBytes = sizeof(float) * data[index].size();
			buf.pAudioData = (BYTE*)(&data[index][0]);

			auto hr = voice->SubmitSourceBuffer(&buf);
			if (FAILED(hr))
			{
				OutputDebugString(_T("\n�g�`�f�[�^�̃o�b�t�@�̒ǉ��F���s\n"));
				threadFlag = false;
				break;
			}

			if (index + 1 >= data.size())
			{
				arrival = true;
				index = 0;
			}
			else
			{
				++index;
			}
		}
	}

	Close(file);

	func = &Wave::Loaded;
}

// �g�`�����ׂēǂݍ��ݏI����Ă���ꍇ�̏���
void Wave::Loaded(void)
{
	XAUDIO2_VOICE_STATE st = {};

	while (threadFlag)
	{
		if (st.BuffersQueued < BUFFER_NUM)
		{
			//�o�b�t�@�ɒǉ�
			XAUDIO2_BUFFER buf = {};
			buf.AudioBytes = sizeof(float) * data[index].size();
			buf.pAudioData = (BYTE*)(&data[index][0]);

			auto hr = voice->SubmitSourceBuffer(&buf);
			if (FAILED(hr))
			{
				OutputDebugString(_T("\n�g�`�f�[�^�̃o�b�t�@�̒ǉ��F���s\n"));
				threadFlag = false;
				break;
			}

			if (index + 1 >= data.size())
			{
				arrival = true;
				index = 0;
			}
			else
			{
				++index;
			}
		}
	}
}

// �񓯊�
void Wave::Stream(void)
{
	(this->*func)();
}

// �Đ�
long Wave::Play(const bool & loop)
{
	if (loop == false && arrival == true)
	{
		OutputDebugString(_T("\nStop()���Ăяo���Ă�������\n"));
		return S_FALSE;
	}

	auto hr = voice->Start();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�g�`�f�[�^�̍Đ��F���s\n"));
	}

	return 0;
}

// ��~
long Wave::Stop(void)
{
	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�g�`�f�[�^�̒�~�F���s\n"));
		return hr;
	}

	arrival = false;

	return hr;
}

// �g�`�f�[�^�̍폜
void Wave::Delete(void)
{
	threadFlag = false;
	if (th.joinable() == true)
	{
		th.join();
	}

	Stop();

	Destroy(voice);

	data.clear();

	index = 0;
}
