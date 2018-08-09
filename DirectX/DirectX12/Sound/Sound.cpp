#include "Sound.h"
#include "../Window/Window.h"
#include "WAVE/WAVE.h"
#include <tchar.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

// 解放マクロ
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// バイト変換
#define BYTE(X) (X) / 8

// プライマリバッファチャンネル
#define PRIMARY_CHANNEL 2
// プライマリバッファサンプリングレート
#define PRIMARY_SAMPLE 44100
// プライマリバッファビット
#define PRIMARY_BIT 16

// コンストラクタ
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

// デストラクタ
Sound::~Sound()
{
	for (auto itr = snd.begin(); itr != snd.end(); ++itr)
	{
		Release(itr->second);
	}
	Release(buffer);
	Release(sound);

	// COMの終了
	CoUninitialize();
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
HRESULT Sound::CreateBuffer(void)
{
	//サウンドバッファ設定用構造体
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
		OutputDebugString(_T("\nプライマリーサウンドバッファの生成：失敗\n"));
	}

	return result;
}

// プライマリサウンドバッファのフォーマットセット
HRESULT Sound::SetFormat(void)
{
	//WAVE情報構造体
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
		OutputDebugString(_T("\nプライマリサウンドバッファのフォーマットセット：失敗\n"));
	}

	return result;
}

// セカンダリバッファの生成
HRESULT Sound::CreateScondly(UINT * index, const WAVE & wave)
{
	//WAVE情報構造体
	WAVEFORMATEX wav = {};
	wav.cbSize          = sizeof(WAVEFORMATEX);
	wav.wFormatTag      = WAVE_FORMAT_PCM;
	wav.nChannels       = wave.channel;
	wav.nSamplesPerSec  = wave.sample;
	wav.wBitsPerSample  = wave.bit;
	wav.nBlockAlign     = wav.nChannels * BYTE(wav.wBitsPerSample);
	wav.nAvgBytesPerSec = wav.nSamplesPerSec * wav.nBlockAlign;

	//サウンドバッファ設定用構造体
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
		OutputDebugString(_T("\nセカンダリバッファの生成：失敗\n"));
	}
	
	return result;
}

// セカンダリバッファのロック
HRESULT Sound::Lock(UINT * index, const WAVE & wave)
{
	LPVOID ptr1, ptr2;
	DWORD size1, size2;
	result = snd[index]->Lock(0, wave.length, &ptr1, &size1, &ptr2, &size2, DSBLOCK_ENTIREBUFFER);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nセカンダリバッファのロック：失敗\n"));
		return result;
	}

	//データのコピー
	memcpy(ptr1, &wave.stereo[0][0], wave.length);
	memcpy(ptr2, &wave.stereo[0][0], wave.length);

	//アンロック
	result = snd[index]->Unlock(ptr1, size1, ptr2, size2);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nセカンダリバッファのアンロック：失敗\n"));
	}

	return result;
}

// WAVEの読み込み
HRESULT Sound::LoadWAVE(UINT & index, const std::string & fileName)
{
	WAVE wave = {};
	if (sound::LoadWave(fileName, wave) != 0)
	{
		OutputDebugString(_T("\nWAVEの読み込み：失敗\n"));
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