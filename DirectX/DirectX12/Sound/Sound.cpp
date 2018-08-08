#include "Sound.h"
#include "../Window/Window.h"
#include <tchar.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

// �o�C�g�ϊ�
#define BYTE(X) (X) / 8

// �R���X�g���N�^
Sound::Sound(std::weak_ptr<Window>win) : 
	win(win), sound(nullptr)
{
	CoInitialize(nullptr);
}

// �f�X�g���N�^
Sound::~Sound()
{
}

// �T�E���h�̐���
HRESULT Sound::CreateSound(void)
{
	result = DirectSoundCreate8(nullptr, &sound, nullptr);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�_�C���N�g�T�E���h�̐����F���s\n"));
	}

	return result;
}

// �������[�h�̃Z�b�g
HRESULT Sound::SetCooperative(void)
{
	result = sound->SetCooperativeLevel(win.lock()->GetHandle(), DSSCL_EXCLUSIVE | DSSCL_PRIORITY);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�������[�h�̃Z�b�g�F���s\n"));
	}

	return result;
}

// �v���C�}���T�E���h�o�b�t�@�̐���
HRESULT Sound::CreateBuffer(UINT* index, const WAVE& wave)
{
	//�t�H�[�}�b�g�ݒ�p�\���̂̐ݒ�
	WAVEFORMATEX wav = {};
	wav.cbSize          = sizeof(WAVEFORMATEX);
	wav.nChannels       = wave.channel;
	wav.nSamplesPerSec  = wave.sample;
	wav.wBitsPerSample  = wave.bit;
	wav.nBlockAlign     = wav.nChannels * BYTE(wav.wBitsPerSample);
	wav.nAvgBytesPerSec = wav.nSamplesPerSec * wav.nBlockAlign;
	wav.wFormatTag      = WAVE_FORMAT_PCM;

	//�T�E���h�o�b�t�@�ݒ�p�\����
	DSBUFFERDESC desc = {};
	desc.dwBufferBytes   = wav.nSamplesPerSec * wav.wBitsPerSample * wav.nChannels * wave.length;
	desc.dwFlags         = DSBCAPS_GETCURRENTPOSITION2;
	desc.dwReserved      = 0;
	desc.dwSize          = sizeof(DSBUFFERDESC);
	desc.guid3DAlgorithm = GUID_NULL;
	desc.lpwfxFormat     = &wav;

	result = sound->CreateSoundBuffer(&desc, &buffer[index], nullptr);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�v���C�}���[�T�E���h�o�b�t�@�̐����F���s\n"));
	}

	return result;
}
