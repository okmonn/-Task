#include "Sound.h"
#include "SoundLoader.h"
#include "../XAudio2/XAudio2.h"
#include "../XAudio2/VoiceCallback.h"
#include "../Func/Func.h"
#include <ks.h>
#include <ksmedia.h>
#include <tchar.h>

// ����}�N��
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }

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
Sound::Sound() : 
	audio(XAudio2::Get()), loader(std::make_shared<SoundLoader>()), threadFlag(true)
{
	snd.clear();
}

// �f�X�g���N�^
Sound::~Sound()
{
	threadFlag = false;

	for (auto itr = snd.begin(); itr != snd.end(); ++itr)
	{
		itr->second.th.join();
	}

	for (auto itr = snd.begin(); itr != snd.end(); ++itr)
	{
		Destroy(itr->second.voice);
	}

	loader.reset();
}

// �\�[�X�{�C�X�̐���
long Sound::Create(int * i, const int& channel, const int& sample, const int& bit)
{
	//�t�H�[�}�b�g�ݒ�p�\����
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

	auto hr = audio.GetAudio()->CreateSourceVoice(&snd[i].voice, (WAVEFORMATEX*)(&fmt), 0, 1.0f, nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�\�[�X�{�C�X�̐����F���s\n"));
	}

	return hr;
}

// �ǂݍ���
long Sound::Load(const std::string & fileName, int & i)
{
	if (loader->Load(fileName) != 0)
	{
		return S_FALSE;
	}

	auto hr = Create(&i, loader->GetChannel(fileName), loader->GetSample(fileName), loader->GetBit(fileName));
	if (FAILED(hr))
	{
		return hr;
	}

	snd[&i].data = loader->GetData(fileName);

	snd[&i].th = std::thread(&Sound::Stream, this, &i);

	return hr;
}

// �񓯊�����
void Sound::Stream(int * i)
{
	XAUDIO2_VOICE_STATE st = {};

	while (threadFlag)
	{
		if (st.BuffersQueued < BUFFER_NUM)
		{
			//�o�b�t�@�ɒǉ�
			XAUDIO2_BUFFER buf = {};
			buf.AudioBytes = sizeof(float) * snd[i].data[snd[i].index].size();
			buf.pAudioData = (BYTE*)(&snd[i].data[snd[i].index][0]);

			auto hr = snd[i].voice->SubmitSourceBuffer(&buf);
			if (FAILED(hr))
			{
				OutputDebugString(_T("\n�g�`�f�[�^���o�b�t�@�ɒǉ��ł��܂���ł���\n"));
				break;
			}

			if (snd[i].index + 1 >= snd[i].data->size())
			{
				if (snd[i].loop == true)
				{
					Stop(*i);
				}
				snd[i].index = 0;
			}
			else
			{
				++snd[i].index;
			}
		}
	}
}

// �Đ�
long Sound::Play(int & i, const bool & loop)
{
	auto hr = snd[&i].voice->Start();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�g�`�f�[�^�̍Đ��F���s\n"));
		return hr;
	}

	snd[&i].loop = loop;

	return  hr;
}

// ��~
long Sound::Stop(int & i)
{
	auto hr = snd[&i].voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�g�`�f�[�^�̒�~�F���s\n"));
	}

	return hr;
}

// �ŏ��̈ʒu�Ƀ��Z�b�g
void Sound::ZeroPos(int & i)
{
	XAUDIO2_VOICE_STATE st = {};
	snd[&i].voice->GetState(&st);

	st.BuffersQueued = 0;

	snd[&i].index = 0;
}