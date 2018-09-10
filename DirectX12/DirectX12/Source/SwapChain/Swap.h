#pragma once
#include "../BaseObj/Obj.h"
#include <dxgi1_6.h>
#include <memory>

class Window;
class Queue;

class Swap :
	public Obj
{
public:
	// コンストラクタ
	Swap(std::weak_ptr<Window>win, std::weak_ptr<Queue>queue);
	// デストラクタ
	~Swap();

	// 裏・表の入れ替え
	void Present(void);

	// スワップチェインの取得
	IDXGISwapChain3* Get(void) const {
		return swap;
	}

	// バックバッファの取得
	UINT GetBack(void) const {
		return bufferCnt;
	}

private:
	// ファクトリーの生成
	HRESULT CreateFactory(void);

	// スワップチェインの生成
	HRESULT CreateSwap(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// コマンドキュー
	std::weak_ptr<Queue>queue;

	// ファクトリー
	IDXGIFactory6* factory;

	// スワップチェイン
	IDXGISwapChain4* swap;

	// バックバッファ数
	UINT bufferCnt;
};

