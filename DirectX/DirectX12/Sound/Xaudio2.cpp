#include "Xaudio2.h"
#define _NOT_USE_D3D12
#include "../DirectX/Obj.h"
#include <tchar.h>
#include <thread>

#pragma comment(lib, "xaudio2.lib")

// コンストラクタ
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

// デストラクタ
Xaudio2::~Xaudio2()
{
	for (auto itr = voice.begin(); itr != voice.end(); ++itr)
	{
		if (itr->second != nullptr)
		{
			result = itr->second->Stop();
			if (FAILED(result))
			{
				OutputDebugString(_T("\n再生の停止：失敗\n"));
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

// 初期化
HRESULT Xaudio2::Init(void)
{
	result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nCOMの初期化：失敗\n"));
	}

	return result;
}

// XAudio2の生成
HRESULT Xaudio2::CreateAudio(void)
{
	result = XAudio2Create(&audio);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nXAudio2の生成：失敗\n"));
	}

	return result;
}

// デバッグのセット
void Xaudio2::SetDebug(void)
{
	XAUDIO2_DEBUG_CONFIGURATION debug = {};
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	audio->SetDebugConfiguration(&debug, 0);
}

// マスターボイスの生成
HRESULT Xaudio2::CreateMaster(void)
{
	result = audio->CreateMasteringVoice(&master);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nマスターボイスの生成：失敗\n"));
	}

	return result;
}

// 読み込み
HRESULT Xaudio2::LoadWAVE(const std::string& fileName, WAVE& wave)
{
	wave.Load(fileName);

	result = audio->CreateSourceVoice(&voice[&wave], &wave.format, 0, 1.0f, &wave.callback);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nソースボイスの生成：失敗\n"));
		return result;
	}

	//再生開始
	result = voice[&wave]->Start();
	if (FAILED(result))
	{
		OutputDebugString(_T("\n再生：失敗\n"));
		return result;
	}

	//バッファー設定用構造体
	XAUDIO2_BUFFER buffer = {};
	buffer.AudioBytes = wave.data.size();
	buffer.pAudioData = wave.data.data();
	buffer.Flags      = XAUDIO2_END_OF_STREAM;

	//バッファーのセット
	result = voice[&wave]->SubmitSourceBuffer(&buffer);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nバッファーのセット：失敗\n"));
	}
	
	return result;
}

// ソースボイスの消去
void Xaudio2::Delete(WAVE & wave)
{
	if (voice[&wave] != nullptr)
	{
		result = voice[&wave]->Stop();
		if (FAILED(result))
		{
			OutputDebugString(_T("\n再生の停止：失敗\n"));
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
