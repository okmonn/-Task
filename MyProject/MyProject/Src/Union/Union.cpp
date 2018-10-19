#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#include "Union.h"
#include "../Window/Window.h"
#include "../Debuger/Debug.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Fence/Fence.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Point/Point.h"
#include "../Line/Line.h"
#include "../Triangle/Triangle.h"
#include "../Texture/TextureLoader.h"
#include "../Texture/Texture.h"
#include "../etc/Release.h"

#pragma comment(lib, "d3d12.lib")

// インプットエレメント
const D3D12_INPUT_ELEMENT_DESC inputs[] = {
	//0
	{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//1
	{ "NORMAL",   0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//2
	{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//3
	{ "COLOR",    0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//4
	{ "ALPHA",    0, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT,          0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//5
	{ "BORN",     0, DXGI_FORMAT::DXGI_FORMAT_R16G16_UINT,        0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//6
	{ "WEIGHT",   0, DXGI_FORMAT::DXGI_FORMAT_R8_UINT,            0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

// コンストラクタ
Union::Union() :
	x(0), y(0)
{
}

// デストラクタ	
Union::~Union()
{
	End();
}

// ウィンドウサイズのセット
void Union::SetWinSize(const unsigned int & x, const unsigned int & y)
{
	this->x = x;
	this->y = y;
}

// ルートシグネチャのインスタンス
void Union::CreateRoot(void)
{
	drwRoot = std::make_shared<Root>(dev, L"Src/Shader/Draw.hlsl");
	texRoot = std::make_shared<Root>(dev, L"Src/Shader/Texture.hlsl");
}

// パイプラインのインスタンス
void Union::CreatePipe(void)
{
	pntPipe = std::make_shared<Pipe>(dev, swap, drwRoot);
	linPipe = std::make_shared<Pipe>(dev, swap, drwRoot);
	triPipe = std::make_shared<Pipe>(dev, swap, drwRoot);
	{
		D3D12_INPUT_ELEMENT_DESC input[] = {
			inputs[0], inputs[3]
		};
		pntPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
		linPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
		triPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}

	texPipe = std::make_shared<Pipe>(dev, swap, texRoot);
	{
		D3D12_INPUT_ELEMENT_DESC input[] = {
			inputs[0], inputs[2], inputs[4]
		};
		texPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}
}
int n = 0;

// クラスのインスタンス
void Union::Start(void)
{
	win  = std::make_shared<Window>(x, y);
	deg  = std::make_shared<Debug>();
	dev  = std::make_shared <Device>();
	que  = std::make_shared<Queue>(dev);
	list = std::make_shared<List>(dev);
	swap = std::make_shared<Swap>(win, que);
	fen  = std::make_shared<Fence>(dev, que);
	ren  = std::make_shared<Render>(dev, list, swap);
	dep  = std::make_shared<Depth>(dev, list);
	con  = std::make_shared<Constant>(dev, list);

	CreateRoot();

	CreatePipe();

	pnt = std::make_shared<Point>(dev, list, con, drwRoot, pntPipe);
	lin = std::make_shared<Line>(dev, list, con, drwRoot, linPipe);
	tri = std::make_shared<Triangle>(dev, list, con, drwRoot, triPipe);

	texLoad = std::make_shared<TextureLoader>(dev);
	tex = std::make_shared<Texture>(dev, list, con, texRoot, texPipe, texLoad);
	tex->Load("avicii.png", n);
}

// メッセージの確認
bool Union::CheckMsg(void)
{
	static MSG msg = {};

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
	list->Reset(nullptr);

	con->UpDataWvp(0.0f);

	list->SetViewport();

	list->SetScissor();

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		swap, ren);

	dep->SetDepth();

	ren->SetRender(*dep->GetHeap());

	tex->Draw(n, 0, 0);
}

// 描画
void Union::Draw(void)
{
	pnt->Draw();
	lin->Draw();
	tri->Draw();
}

// 頂点のリセット
void Union::Reset(void)
{
	pnt->Reset();
	lin->Reset();
	tri->Reset();
}

// 描画実行
void Union::Do(void)
{
	Draw();

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
		swap, ren);

	list->Close();

	//コマンド実行.
	ID3D12CommandList* ppCmdLists[] = {
		list->GetList(),
	};
	que->Execute(ppCmdLists, _countof(ppCmdLists));

	Reset();

	swap->Present();

	fen->Wait();
}

// 終了
void Union::End(void)
{
	pnt.reset();
	pntPipe.reset();
	texPipe.reset();
	drwRoot.reset();
	texRoot.reset();
	con.reset();
	dep.reset();
	ren.reset();
	fen.reset();
	swap.reset();
	list.reset();
	que.reset();
	dev.reset();
	deg.reset();
	win.reset();
}
