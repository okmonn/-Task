#pragma once
#include "../DirectX/Obj.h"
#include "WAVE/WAVE.h"
#include <dsound.h>
#include <memory>
#include <map>

class Window;

class Sound :
	public Obj
{
public:
	// コンストラクタ
	Sound(std::weak_ptr<Window>win);
	// デストラクタ
	~Sound();

private:
	// サウンドの生成
	HRESULT CreateSound(void);

	// 協調モードのセット
	HRESULT SetCooperative(void);

	// プライマリサウンドバッファの生成
	HRESULT CreateBuffer(UINT* index, const WAVE& wave);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ダイレクトサウンド
	LPDIRECTSOUND8 sound;

	// セカンダリーバッファ
	std::map<UINT*, LPDIRECTSOUNDBUFFER>buffer;
};

