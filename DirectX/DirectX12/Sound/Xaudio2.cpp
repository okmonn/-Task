#include "Xaudio2.h"
#define _NOT_USE_D3D12
#include "../DirectX/Obj.h"
#include <tchar.h>
#include <thread>

#pragma comment(lib, "xaudio2.lib")

// �R���X�g���N�^
Xaudio2::Xaudio2() : 
	result(S_OK), audio(nullptr), master(nullptr)
{
	voice.clear();

	Init();
	CreateAudio();
#ifdef _DEBUG
	SetDebug();
#endif
	CreateMaster();
}

// �f�X�g���N�^
Xaudio2::~Xaudio2()
{
	for (auto itr = voice.begin(); itr != voice.end(); ++itr)
	{
		if (itr->second != nullptr)
		{
			result = itr->second->Stop();
			if (FAILED(result))
			{
				OutputDebugString(_T("\n�Đ��̒�~�F���s\n"));
			}
			itr->second->DestroyVoice();
		}
	}
	if (master != nullptr)
	{
		master->DestroyVoice();
	}
	Release(audio);
	CoUninitialize();
}

// ������
HRESULT Xaudio2::Init(void)
{
	result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nCOM�̏������F���s\n"));
	}

	return result;
}

// XAudio2�̐���
HRESULT Xaudio2::CreateAudio(void)
{
	result = XAudio2Create(&audio);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nXAudio2�̐����F���s\n"));
	}

	return result;
}

// �f�o�b�O�̃Z�b�g
void Xaudio2::SetDebug(void)
{
	XAUDIO2_DEBUG_CONFIGURATION debug = {};
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	audio->SetDebugConfiguration(&debug, 0);
}

// �}�X�^�[�{�C�X�̐���
HRESULT Xaudio2::CreateMaster(void)
{
	result = audio->CreateMasteringVoice(&master);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�}�X�^�[�{�C�X�̐����F���s\n"));
	}

	return result;
}

// �ǂݍ���
HRESULT Xaudio2::LoadWAVE(const std::string& fileName, WAVE& wave)
{
	wave.Load(fileName);

	result = audio->CreateSourceVoice(&voice[&wave], &wave.format, 0, 1.0f, &wave.callback);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�\�[�X�{�C�X�̐����F���s\n"));
		return result;
	}

	//�Đ��J�n
	result = voice[&wave]->Start();
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�Đ��F���s\n"));
		return result;
	}

	//�o�b�t�@�[�ݒ�p�\����
	XAUDIO2_BUFFER buffer = {};
	buffer.AudioBytes = wave.data.size();
	buffer.pAudioData = wave.data.data();
	buffer.Flags      = XAUDIO2_END_OF_STREAM;

	//�o�b�t�@�[�̃Z�b�g
	result = voice[&wave]->SubmitSourceBuffer(&buffer);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�o�b�t�@�[�̃Z�b�g�F���s\n"));
	}
	
	return result;
}

// �\�[�X�{�C�X�̏���
void Xaudio2::Delete(WAVE & wave)
{
	if (voice[&wave] != nullptr)
	{
		result = voice[&wave]->Stop();
		if (FAILED(result))
		{
			OutputDebugString(_T("\n�Đ��̒�~�F���s\n"));
		}
		voice[&wave]->DestroyVoice();
	}

	for (auto itr = voice.begin(); itr != voice.end();)
	{
		if (itr->first == &wave)
		{
			itr = voice.erase(itr);
			break;
		}
		else
		{
			++itr;
		}
	}
}
