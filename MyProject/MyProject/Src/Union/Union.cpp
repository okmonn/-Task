#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#include "Union.h"
#include "../Window/Window.h"
#include "../Debuger/Debug.h"
#include "../Input/Input.h"
#include "../Sound/Sound.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Fence/Fence.h"
#include "../Render/Render.h"
#include "../MultiRender/FirstRender.h"
#include "../Depth/Depth.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Point/Point.h"
#include "../Line/Line.h"
#include "../Triangle/Triangle.h"
#include "../Texture/Texture.h"
#include "../Model/Model.h"
#include "../etc/Release.h"

#pragma comment(lib, "d3d12.lib")

//クリアカラー
const FLOAT color[] = {
	1.0f,
	0.0f,
	0.0f,
	0.0f
};

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
	mdlRoot = std::make_shared<Root>(dev, L"Src/Shader/Model.hlsl");
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

	mdlPipe = std::make_shared<Pipe>(dev, swap, mdlRoot);
	{
		D3D12_INPUT_ELEMENT_DESC input[] = {
			inputs[0], inputs[1], inputs[2], inputs[5], inputs[6]
		};
		mdlPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true);
	}
}

// クラスのインスタンス
void Union::Start(void)
{
	win   = std::make_shared<Window>(x, y);
	deg   = std::make_shared<Debug>();
	input = std::make_shared<Input>(win);
	snd   = std::make_shared<Sound>();
	dev   = std::make_shared <Device>();
	que   = std::make_shared<Queue>(dev);
	list  = std::make_shared<List>(dev);
	swap  = std::make_shared<Swap>(win, que);
	fen   = std::make_shared<Fence>(dev, que);
	ren   = std::make_shared<Render>(dev, list, swap);
	dep   = std::make_shared<Depth>(dev, list);
	con   = std::make_shared<Constant>(dev, list);

	CreateRoot();

	CreatePipe();

	first = std::make_shared<FirstRender>(dev, list, ren, texRoot, texPipe);

	pnt = std::make_shared<Point>(dev, list, con, drwRoot, pntPipe);
	lin = std::make_shared<Line>(dev, list, con, drwRoot, linPipe);
	tri = std::make_shared<Triangle>(dev, list, con, drwRoot, triPipe);
	tex = std::make_shared<Texture>(dev, list, con, texRoot, texPipe);

	model = std::make_shared<Model>(dev, list, con, mdlRoot, mdlPipe, tex);
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

// WVPの更新
void Union::ChangeWVP(const float & eyeX, const float & eyeY, const float & eyeZ, const float & targetX, const float & targetY, 
	const float & targetZ, const float & upX, const float & upY, const float & upZ)
{
	con->ChangeWvp(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, upX, upY, upZ);
}

// キー入力
bool Union::CheckKey(const int & i)
{
	return input->CheckKey(i);
}

// トリガー入力
bool Union::CheckTriger(const int & i)
{
	return input->Triger(i);
}

// サウンドの読み込み
void Union::LoadSnd(const std::string & fileName, int & i)
{
	snd->Load(fileName, i);
}

// サウンドの再生
void Union::Play(int & i, const bool & loop)
{
	snd->Play(i, loop);
}

// サウンドの停止
void Union::Stop(int & i)
{
	snd->Stop(i);
}

// 画像の読み込み
void Union::LoadImg(const std::string & fileName, int& i)
{
	tex->Load(fileName, i);
}

// PMDの読み込み
void Union::LoadPmd(const std::string & fileName, int & i)
{
	model->Load(fileName, i);
}

// アニメーションのアタッチ
void Union::Attach(const std::string & fileName, int & i)
{
	model->Attach(fileName, i);
}

// 描画準備
void Union::Set(void)
{
	list->Reset(nullptr);

	con->UpDataWvp(0.0f);

	list->SetViewport();

	list->SetScissor();
	
	dep->SetDepth();

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		swap, ren);

	ren->SetRender(*dep->GetHeap(), color);
}

// ポイント描画
void Union::DrawPoint(const float & x, const float & y, const float & r, const float & g, const float & b, const float & alpha)
{
	pnt->AddVertexPoint(x, y, r, g, b, alpha);
}

// ライン描画
void Union::DrawLine(const float & x1, const float & y1, const float & x2, const float & y2, const float & r, const float & g, const float & b, const float & alpha)
{
	lin->AddVertexPoint(x1, y1, x2, y2, r, g, b, alpha);
}

// トライアングル描画
void Union::DrawTriangle(const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, 
	const float & r, const float & g, const float & b, const float & alpha)
{
	tri->AddVertexPoint(x1, y1, x2, y2, x3, y3, r, g, b, alpha);
}

// 画像の描画
void Union::DrawImg(int& i, const float& x, const float& y, const float& alpha, const int& turnX, const int& turnY)
{
	tex->Draw(i, x, y, alpha, turnX, turnY);
}

// 画像の描画・サイズ指定
void Union::DrawSizeImg(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, const float & alpha, const int & turnX, const int & turnY)
{
	tex->DrawSize(i, x, y, sizeX, sizeY, alpha, turnX, turnY);
}

// 画像の描画・サイズ・範囲指定
void Union::DrawRectImg(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, 
	const float & rectX, const float & rectY, const float & rectSizeX, const float & rectSizeY, const float & alpha, const int & turnX, const int & turnY)
{
	tex->DrawRectSize(i, x, y, sizeX, sizeY, rectX, rectY, rectSizeX, rectSizeY, alpha, turnX, turnY);
}

// 画像の描画・4点指定
void Union::DrawFreelyImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2,
	const float & x3, const float & y3, const float & x4, const float & y4, const float & alpha, const int & turnX, const int & turnY)
{
	tex->FreelyDraw(i, x1, y1, x2, y2, x3, y3, x4, y4, alpha, turnX, turnY);
}

// 画像の描画・4点・範囲指定
void Union::DrawFreelyRectImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & x4, const float & y4, const float & rectX, const float & rectY, const float & rectSizeX, const float & rectSizeY, const float & alpha, const int & turnX, const int & turnY)
{
	tex->FreelyDrawRect(i, x1, y1, x2, y2, x3, y3, x4, y4, rectX, rectY, rectSizeX, rectSizeY, alpha, turnX, turnY);
}

// PMDのアニメーション時間のリセット
void Union::ResetAnim(int & i)
{
	model->ResetAnim(i);
}

// PMDのアニメーション
void Union::Animation(int & i, const bool& loop, const float & animSpeed)
{
	model->Animation(i, loop, animSpeed);
}

// アニメーションの終了確認
bool Union::CheckEndAnim(int & i)
{
	return model->CheckEndAnim(i);
}

// PMDの描画
void Union::DrawPmd(int & i)
{
	model->Draw(i);
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

	fen->Wait();


	/*list->Reset(nullptr);

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		swap, ren);

	ren->SetRender(*dep->GetHeap(), color);

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
		swap, ren);

	list->Close();

	swap->Present();

	fen->Wait();*/
}

// サウンドの削除
void Union::DeleteSnd(int & i)
{
	snd->DeleteSnd(i);
}

// 画像の削除
void Union::DeleteImg(int & i)
{
	tex->DeleteImg(i);
}

// モデルの削除
void Union::DeleteMdl(int & i)
{
	model->DeleteMdl(i);
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
	snd.reset();
	input.reset();
	deg.reset();
	win.reset();
}
