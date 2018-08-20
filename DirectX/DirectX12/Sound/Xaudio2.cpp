#include "Xaudio2.h"
#define _NOT_USE_D3D12
#include "../DirectX/Obj.h"
#include "WAVE.h"
#include <tchar.h>

#pragma comment(lib, "xaudio2.lib")

// コンストラクタ
Xaudio2::Xaudio2() :
	result(S_OK), audio(nullptr), master(nullptr)
{
	wave.clear();
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
HRESULT Xaudio2::LoadWAVE(UINT& index, const std::string& fileName)
{
	wave[&index].Load(fileName);

	result = audio->CreateSourceVoice(&voice[&wave[&index]], &wave[&index].format, 0, 1.0f, &wave[&index].callback);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nソースボイスの生成：失敗\n"));
		return result;
	}

	//再生開始
	result = voice[&wave[&index]]->Start();
	if (FAILED(result))
	{
		OutputDebugString(_T("\n再生：失敗\n"));
		return result;
	}

	return result;
}

// 再生開始
HRESULT Xaudio2::Play(UINT& index)
{
	if (wave[&index].GetEnd() == true)
	{
		return S_FALSE;
	}

	XAUDIO2_VOICE_STATE state = {};
	voice[&wave[&index]]->GetState(&state);
	if (state.BuffersQueued <= wave[&index].data.size() - 1)
	{
		wave[&index].Load();

		//バッファー設定用構造体
		XAUDIO2_BUFFER buffer = {};
		buffer.AudioBytes = wave[&index].data[wave[&index].GetIndex()].size();
		buffer.pAudioData = wave[&index].data[wave[&index].GetIndex()].data();
		buffer.Flags = XAUDIO2_END_OF_STREAM;

		//バッファーのセット
		result = voice[&wave[&index]]->SubmitSourceBuffer(&buffer);
		if (FAILED(result))
		{
			OutputDebugString(_T("\nバッファーのセット：失敗\n"));
		}
	}

	return result;
}

// 再生停止
HRESULT Xaudio2::Stop(UINT& index)
{
	result = voice[&wave[&index]]->Stop();
	if (FAILED(result))
	{
		OutputDebugString(_T("\n再生停止：失敗\n"));
	}

	return result;
}

// ソースボイスの消去
void Xaudio2::Delete(UINT& index)
{
	if (voice[&wave[&index]] != nullptr)
	{
		result = voice[&wave[&index]]->Stop();
		if (FAILED(result))
		{
			OutputDebugString(_T("\n再生の停止：失敗\n"));
		}
		voice[&wave[&index]]->DestroyVoice();
	}

	for (auto itr = voice.begin(); itr != voice.end();)
	{
		if (itr->first == &wave[&index])
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
