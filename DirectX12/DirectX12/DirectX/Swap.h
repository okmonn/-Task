#pragma once
#include "Obj.h"
#include <dxgi1_4.h>
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

	// インターフェースファクトリーの取得
	IDXGISwapChain3* Get(void) const {
		return swap;
	}

	// バックバッファ数の取得
	UINT GetBack(void) const {
		return bufferCnt;
	}

private:
	// ファクトリーの生成
	HRESULT CreateFactory(void);

	// スワップチェインの生成
	HRESULT Create(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// コマンドリスト
	std::weak_ptr<Queue>queue;

	//インターフェースファクトリー
	IDXGIFactory4* factory;

	//スワップチェイン
	IDXGISwapChain3* swap;

	//バックバッファ数
	UINT bufferCnt;

};

