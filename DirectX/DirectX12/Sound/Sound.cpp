#include "Sound.h"
#include "../Window/Window.h"
#include "WAVE/WAVE.h"
#include <tchar.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

// ����}�N��
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// �o�C�g�ϊ�
#define BYTE(X) (X) / 8

// �v���C�}���o�b�t�@�`�����l��
#define PRIMARY_CHANNEL 2
// �v���C�}���o�b�t�@�T���v�����O���[�g
#define PRIMARY_SAMPLE 44100
// �v���C�}���o�b�t�@�r�b�g
#define PRIMARY_BIT 16

// �R���X�g���N�^
Sound::Sound(std::weak_ptr<Window>win) : 
	win(win), result(S_OK), sound(nullptr), buffer(nullptr)
{
	snd.clear();

	CoInitialize(nullptr);

	CreateSound();
	SetCooperative();
	CreateBuffer();
	SetFormat();
}

// �f�X�g���N�^
Sound::~Sound()
{
	for (auto itr = snd.begin(); itr != snd.end(); ++itr)
	{
		Release(itr->second);
	}
	Release(buffer);
	Release(sound);

	// COM�̏I��
	CoUninitialize();
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
HRESULT Sound::CreateBuffer(void)
{
	//�T�E���h�o�b�t�@�ݒ�p�\����
	DSBUFFERDESC desc = {};
	desc.dwBufferBytes   = 0;
	desc.dwFlags         = DSBCAPS_PRIMARYBUFFER;
	desc.dwReserved      = 0;
	desc.dwSize          = sizeof(DSBUFFERDESC);
	desc.guid3DAlgorithm = GUID_NULL;
	desc.lpwfxFormat     = nullptr;

	result = sound->CreateSoundBuffer(&desc, &buffer, nullptr);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�v���C�}���[�T�E���h�o�b�t�@�̐����F���s\n"));
	}

	return result;
}

// �v���C�}���T�E���h�o�b�t�@�̃t�H�[�}�b�g�Z�b�g
HRESULT Sound::SetFormat(void)
{
	//WAVE���\����
	WAVEFORMATEX wav = {};
	wav.cbSize          = sizeof(WAVEFORMATEX);
	wav.wFormatTag      = WAVE_FORMAT_PCM;
	wav.nChannels       = PRIMARY_CHANNEL;
	wav.nSamplesPerSec  = PRIMARY_SAMPLE;
	wav.wBitsPerSample  = PRIMARY_BIT;
	wav.nBlockAlign     = wav.nChannels * BYTE(wav.wBitsPerSample);
	wav.nAvgBytesPerSec = wav.nSamplesPerSec * wav.nBlockAlign;

	result = buffer->SetFormat(&wav);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�v���C�}���T�E���h�o�b�t�@�̃t�H�[�}�b�g�Z�b�g�F���s\n"));
	}

	return result;
}

// �Z�J���_���o�b�t�@�̐���
HRESULT Sound::CreateScondly(UINT * index, const WAVE & wave)
{
	//WAVE���\����
	WAVEFORMATEX wav = {};
	wav.cbSize          = sizeof(WAVEFORMATEX);
	wav.wFormatTag      = WAVE_FORMAT_PCM;
	wav.nChannels       = wave.channel;
	wav.nSamplesPerSec  = wave.sample;
	wav.wBitsPerSample  = wave.bit;
	wav.nBlockAlign     = wav.nChannels * BYTE(wav.wBitsPerSample);
	wav.nAvgBytesPerSec = wav.nSamplesPerSec * wav.nBlockAlign;

	//�T�E���h�o�b�t�@�ݒ�p�\����
	DSBUFFERDESC desc = {};
	desc.dwBufferBytes   = wav.nSamplesPerSec * wav.wBitsPerSample * wav.nChannels /** wave.length*/;
	desc.dwFlags         = 0;
	desc.dwReserved      = 0;
	desc.dwSize          = sizeof(DSBUFFERDESC);
	desc.guid3DAlgorithm = GUID_NULL;
	desc.lpwfxFormat     = &wav;

	result = sound->CreateSoundBuffer(&desc, &snd[index], nullptr);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�Z�J���_���o�b�t�@�̐����F���s\n"));
	}
	
	return result;
}

// �Z�J���_���o�b�t�@�̃��b�N
HRESULT Sound::Lock(UINT * index, const WAVE & wave)
{
	LPVOID ptr1, ptr2;
	DWORD size1, size2;
	result = snd[index]->Lock(0, wave.length, &ptr1, &size1, &ptr2, &size2, DSBLOCK_ENTIREBUFFER);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�Z�J���_���o�b�t�@�̃��b�N�F���s\n"));
		return result;
	}

	//�f�[�^�̃R�s�[
	memcpy(ptr1, &wave.stereo[0][0], wave.length);
	memcpy(ptr2, &wave.stereo[0][0], wave.length);

	//�A�����b�N
	result = snd[index]->Unlock(ptr1, size1, ptr2, size2);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�Z�J���_���o�b�t�@�̃A�����b�N�F���s\n"));
	}

	return result;
}

// WAVE�̓ǂݍ���
HRESULT Sound::LoadWAVE(UINT & index, const std::string & fileName)
{
	WAVE wave = {};
	if (sound::LoadWave(fileName, wave) != 0)
	{
		OutputDebugString(_T("\nWAVE�̓ǂݍ��݁F���s\n"));
		return S_FALSE;
	}

	UINT* n = &index;

	result = CreateScondly(n, wave);
	if (FAILED(result))
	{
		return result;
	}

	result = Lock(n, wave);

	return result;
}