#include "Union.h"
#include "../Window/Window.h"
#ifdef _DEBUG
#include "Debug.h"
#endif
#include "Device.h"
#include "Command/Queue.h"
#include "Command/List.h"
#include "Swap.h"
#include "Descriptor/Render.h"
#include "Descriptor/Depth.h"
#include "Fence.h"

#pragma comment (lib, "d3d12.lib")

// コンストラクタ
Union::Union()
{
	msg = {};
	viewPort = {};
	scissor = {};
}

// デストラクタ
Union::~Union()
{
}

// ウィンドウのサイズセット
void Union::ChangeWindowSize(UINT x, UINT y)
{
	this->x = x;
	this->y = y;
}

// クラスのインスタンス化
void Union::Create(void)
{
	win    = std::make_shared<Window>(x, y);
#ifdef _DEBUG
	debug  = std::make_shared<Debug>();
#endif
	dev    = std::make_shared<Device>();
	queue  = std::make_shared<Queue>(dev);
	list   = std::make_shared<List>(dev);
	swap   = std::make_shared<Swap>(win, queue);
	render = std::make_shared<Render>(dev, list, swap);
	depth  = std::make_shared<Depth>(win, dev, list, swap);
	std::make_shared<Fence>(dev, queue);
}



// メッセージの確認
bool Union::CheckMsg(void)
{
	if (msg.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換
			TranslateMessage(&msg);
			//1つのウィドウプロシージャにメッセージを送出する
			DispatchMessage(&msg);
		}
	}

	return true;
}

// 描画準備
void Union::Set(void)
{
}

// 実行
void Union::Do(void)
{
}
