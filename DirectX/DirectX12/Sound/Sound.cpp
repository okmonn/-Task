#include "Sound.h"
#include "../Window/Window.h"
#include <tchar.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

// バイト変換
#define BYTE(X) (X) / 8

// コンストラクタ
Sound::Sound(std::weak_ptr<Window>win) : 
	win(win), sound(nullptr)
{
	CoInitialize(nullptr);
}

// デストラクタ
Sound::~Sound()
{
}

// サウンドの生成
HRESULT Sound::CreateSound(void)
{
	result = DirectSoundCreate8(nullptr, &sound, nullptr);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nダイレクトサウンドの生成：失敗\n"));
	}

	return result;
}

// 協調モードのセット
HRESULT Sound::SetCooperative(void)
{
	result = sound->SetCooperativeLevel(win.lock()->GetHandle(), DSSCL_EXCLUSIVE | DSSCL_PRIORITY);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n協調モードのセット：失敗\n"));
	}

	return result;
}

// プライマリサウンドバッファの生成
HRESULT Sound::CreateBuffer(UINT* index, const WAVE& wave)
{
	//フォーマット設定用構造体の設定
	WAVEFORMATEX wav = {};
	wav.cbSize          = sizeof(WAVEFORMATEX);
	wav.nChannels       = wave.channel;
	wav.nSamplesPerSec  = wave.sample;
	wav.wBitsPerSample  = wave.bit;
	wav.nBlockAlign     = wav.nChannels * BYTE(wav.wBitsPerSample);
	wav.nAvgBytesPerSec = wav.nSamplesPerSec * wav.nBlockAlign;
	wav.wFormatTag      = WAVE_FORMAT_PCM;

	//サウンドバッファ設定用構造体
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
		OutputDebugString(_T("\nプライマリーサウンドバッファの生成：失敗\n"));
	}

	return result;
}
