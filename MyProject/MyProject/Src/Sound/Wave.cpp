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
	audio(XAudio2::Get()), back(std::make_unique<VoiceCallback>()), voice(nullptr), file(nullptr), channel(0), sample(0), bit(0), 
	start(0), index(0)
{
	data.clear();

	func = &Wave::Loading;
}

// �f�X�g���N�^
Wave::~Wave()
{
	Destroy(voice);
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
		return;
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

	//�\�[�X�{�C�X����
	if (FAILED(Create()))
	{
		Close(file);
		return -1;
	}

	//�g�`�f�[�^�̊J�n�ʒu�擾
	start = (riff.size + sizeof(riff.id) + sizeof(riff.size)) - data.size;

	return 0;
}

// �g�`�����ׂēǂݍ��ݏI����ĂȂ��ꍇ�̏���
void Wave::Loading(void)
{
	XAUDIO2_VOICE_STATE st = {};

	while (true)
	{
		voice->GetState(&st);
		if (st.BuffersQueued < BUFFER_NUM)
		{
			//�z��̃������m��
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

			//�g�`�̃t�@�C���ǂݍ��݂����s
			if (flag != 0)
			{
				threadFlag = false;
				Close(file);
				break;
			}

			//sound::Delay(stream[index], fmt.Format.nSamplesPerSec, bit, 0.3f, 0.05f, 10);

			//�o�b�t�@�ɒǉ�
			XAUDIO2_BUFFER buf = {};
			buf.AudioBytes = sizeof(float) * stream[index].size();
			buf.pAudioData = (BYTE*)(&stream[index][0]);

			auto hr = voice->SubmitSourceBuffer(&buf);
			if (FAILED(hr))
			{
				OutputDebugString(_T("\n�o�b�t�@�̒ǉ��F���s\n"));
				threadFlag = false;
				Close(file);
				break;
			}

			if (feof(file) != 0)
			{
				//���[�v
				if (loop == true)
				{
					fseek(file, startPos, SEEK_SET);
				}
				else
				{
					//�o�b�t�@����ɂȂ����Ƃ�
					if (st.BuffersQueued <= 0)
					{
						Stop();
						fseek(file, startPos, SEEK_SET);
					}
				}
			}

			//�z��ԍ��̍X�V
			index = (index >= stream.size() - 1) ? 0 : 1;
		}
	}
}

// �g�`�����ׂēǂݍ��ݏI����Ă���ꍇ�̏���
void Wave::Loaded(void)
{
}

