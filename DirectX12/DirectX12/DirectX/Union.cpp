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
#include "Descriptor/Constant.h"
#include "RootSignature.h"
#include <tchar.h>

#pragma comment (lib, "d3d12.lib")

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
	win      = std::make_shared<Window>(x, y);
#ifdef _DEBUG
	debug    = std::make_shared<Debug>();
#endif
	dev      = std::make_shared<Device>();
	queue    = std::make_shared<Queue>(dev);
	list     = std::make_shared<List>(dev);
	swap     = std::make_shared<Swap>(win, queue);
	render   = std::make_shared<Render>(dev, swap);
	depth    = std::make_shared<Depth>(win, dev, swap);
	constant = std::make_shared<Constant>(win, dev, swap);
	root     = std::make_shared<RootSignature>(dev, _T("Shader2D.hlsl"), "BasicVS", "BasicPS");
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
