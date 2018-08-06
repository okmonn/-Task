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
#include "Pipe.h"
#include "Fence.h"
#include "Barrier.h"
#include "Texture\Texture.h"
#include <tchar.h>

#pragma comment (lib, "d3d12.lib")

UINT m = 0;
UINT n = 0;

// コンストラクタ
Union::Union() : x(x), y(y)
{
	msg = {};
	viewPort = {};
	scissor = {};
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
	render   = std::make_shared<Render>(dev, list, swap);
	depth    = std::make_shared<Depth>(win, dev, list, swap);
	constant = std::make_shared<Constant>(win, dev, list, swap);
	root     = std::make_shared<RootSignature>(dev/*, _T("Shader2D.hlsl"), "BasicVS", "BasicPS"*/);
	root->ComVer(_T("Shader2D.hlsl"), "BasicVS");
	root->ComPix(_T("Shader2D.hlsl"), "BasicPS");
	pipe     = std::make_shared<Pipe>(dev, swap, root);
	fence    = std::make_shared<Fence>(dev, queue);
	barrier  = std::make_shared<Barrier>(list, swap, render);
	tex = std::make_shared<Texture>(win, dev, list, swap);
	tex->LoadWIC(m, "img/sample.png");
	tex->LoadWIC(n, "img/sample.png");

	viewPort = { 0, 0, (FLOAT)this->win->GetX(), (FLOAT)this->win->GetY(), 0, 1 };
	scissor  = { 0, 0, static_cast<LONG>(this->win->GetX()), static_cast<LONG>(this->win->GetY()) };
}

// ウィンドウのサイズセット
void Union::ChangeWindowSize(UINT x, UINT y)
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

// 描画準備
void Union::Set(void)
{
	//WVP更新
	constant->UpDataWVP();

	//リセット
	{
		list->GetAllo()->Reset();
		list->GetList()->Reset(list->GetAllo(), pipe->Get());
	}

	//ルートシグネチャのセット
	list->GetList()->SetGraphicsRootSignature(root->Get());

	//パイプラインのセット
	list->GetList()->SetPipelineState(pipe->Get());

	//定数バッファのセット
	constant->SetConstant();

	//ビューのセット
	list->GetList()->RSSetViewports(1, &viewPort);

	//シザーのセット
	list->GetList()->RSSetScissorRects(1, &scissor);

	//バリアの設置
	barrier->SetBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//レンダーターゲットのセット
	render->SetRender(depth->GetHeap());

	//深度ステンシルのセット
	depth->SetDepth();

	tex->Draw(m/*, Vec2f(0,0), Vec2f(100,100)*/);

	tex->Draw(n
);
}

// 実行
void Union::Do(void)
{
	//バリアの設置
	barrier->SetBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//コマンドリストの記録終了
	list->GetList()->Close();

	//コマンドリストの実行
	{
		//リストの配列
		ID3D12CommandList *commandList[] = { list->GetList() };
		//配列でない場合：queue->ExecuteCommandLists(1, (ID3D12CommandList*const*)&list);
		queue->Get()->ExecuteCommandLists(_countof(commandList), commandList);
	}

	//裏、表画面を反転
	swap->Get()->Present(1, 0);

	//待機
	fence->Wait();
}
