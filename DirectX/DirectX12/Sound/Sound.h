#pragma once
#define DIRECTSOUND_VERSION 0x0800
#include <dsound.h>
#include <memory>
#include <string>
#include <map>

class Window;
class WAVE;

class Sound
{
public:
	// コンストラクタ
	Sound(std::weak_ptr<Window>win);
	// デストラクタ
	~Sound();

	// WAVEの読み込み
	HRESULT LoadWAVE(UINT& index, const std::string& fileName);

private:
	// サウンドの生成
	HRESULT CreateSound(void);

	// 協調モードのセット
	HRESULT SetCooperative(void);

	// プライマリサウンドバッファの生成
	HRESULT CreateBuffer(void);

	// プライマリサウンドバッファのフォーマットセット
	HRESULT SetFormat(void);

	// セカンダリバッファの生成
	HRESULT CreateScondly(UINT* index, const WAVE& wave);

	// セカンダリバッファのロック
	HRESULT Lock(UINT* index, const WAVE& wave);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// 参照結果
	HRESULT result;

	// ダイレクトサウンド
	LPDIRECTSOUND8 sound;

	// プライマリーサウンドバッファ
	LPDIRECTSOUNDBUFFER buffer;

	// セカンダリーバッファ
	std::map<UINT*, LPDIRECTSOUNDBUFFER>snd;
};

