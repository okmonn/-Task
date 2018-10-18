#pragma once
#include <memory>

struct IDXGISwapChain4;

class Union;
class Window;
class Queue;

class Swap
{
public:
	// コンストラクタ
	Swap(std::weak_ptr<Window>win, std::weak_ptr<Queue>queue);
	// デストラクタ
	~Swap();

	// スワップチェイン
	IDXGISwapChain4* Get(void) const {
		return swap;
	}
	// バックバッファ数の取得
	unsigned int GetCnt(void) const {
		return cnt;
	}

private:
	// スワップチェインの生成
	long Create(void);


	// ユニオン
	Union& un;

	// ウィンドウ
	std::weak_ptr<Window>win;

	// コマンドキュー
	std::weak_ptr<Queue>queue;

	// スワップチェイン
	IDXGISwapChain4* swap;

	// バックバッファの数
	unsigned int cnt;
};
