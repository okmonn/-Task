#include "XAudio2.h"
#include <xaudio2.h>
#include "../etc/Release.h"

#pragma comment(lib, "xaudio2.lib")

// 解放マクロ
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }

// コンストラクタ
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
	Init();
}

// デストラクタ
XAudio2::~XAudio2()
{
	Destroy(mastering);
	Release(audio);

	CoUninitialize();
}

// COMの初期化
long XAudio2::InitCom(void)
{
	auto hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nCOMの初期化：失敗\n"));
	}

	return hr;
}

// XAudio2の生成
long XAudio2::CreateAudio(void)
{
	auto hr = XAudio2Create(&audio);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nXAudio2の生成：失敗\n"));
	}

	return hr;
}

// マスタリングボイスの生成
long XAudio2::CreateMastering(void)
{
	auto hr = audio->CreateMasteringVoice(&mastering);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nマスタリングボイスの生成：失敗\n"));
	}

	return hr;
}

// 初期化
void XAudio2::Init(void)
{
	InitCom();
	CreateAudio();
	CreateMastering();
}
