#include "Union.h"
#include "../Window/Window.h"
#ifdef _DEBUG
#include "Debug.h"
#endif
#include "Device.h"
#include "Command/Allocator.h"
#include "Command/Queue.h"
#include "Command/List.h"
#include "Swap.h"

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib,"dxgi.lib")

// コンストラクタ
Union::Union() : x(x), y(y)
{
	msg = {};
}

// デストラクタ
Union::~Union()
{
}

// クラスのインスタンス化
void Union::Create(void)
{
	win = std::make_shared<Window>(x, y);
#ifdef _DEBUG
	debug = std::make_shared<Debug>();
#endif
	dev = std::make_shared<Device>();
	allo = std::make_shared<Allocator>(dev);
	queue = std::make_shared<Queue>(dev);
	list = std::make_shared<List>(dev, allo);
	swap = std::make_shared<Swap>(win, queue);
}

// ウィンドウのサイズセット
void Union::ChangeWindowSize(unsigned int x, unsigned int y)
{
	this->x = x;
	this->y = y;
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
