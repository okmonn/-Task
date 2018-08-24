#include "Union.h"
#include "../Window/Window.h"
#include "../Sound/Xaudio2.h"
#include "../Sound/MIDI_IN.h"
#include "../Input/Input.h"
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
#include "Root.h"
#include "Pipe.h"
#include "Descriptor/Constant.h"
#include "Texture/Texture.h"
#include <tchar.h>

#pragma comment(lib, "d3d12.lib")

// コンストラクタ
Union::Union()
{
	msg = {};
	viewPort = {};
	scissor = {};
	barrier = {};
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
	win      = std::make_shared<Window>(x, y);
	audio    = std::make_shared<Xaudio2>();
	in       = std::make_shared<MIDI_IN>();
	input    = std::make_shared<Input>(win);
#ifdef _DEBUG
	debug    = std::make_shared<Debug>();
#endif
	dev      = std::make_shared<Device>();
	queue    = std::make_shared<Queue>(dev);
	list     = std::make_shared<List>(dev);
	swap     = std::make_shared<Swap>(win, queue);
	render   = std::make_shared<Render>(dev, list, swap);
	depth    = std::make_shared<Depth>(win, dev, list, swap);
	fence    = std::make_shared<Fence>(dev, queue);
	root     = std::make_shared<Root>(dev);
	root->ComVertex(L"Shader/BasicShader.hlsl", "VS");
	root->ComPixel(L"Shader/BasicShader.hlsl", "PS");
	pipe     = std::make_shared<Pipe>(dev, swap, root);
	constant = std::make_shared <Constant>(win, dev, list);
	tex      = std::make_shared<Texture>(dev, list);

	ViewPort();
	Scissor();
}

// ビューポートのセット
void Union::ViewPort(void)
{
	viewPort.Height = static_cast<FLOAT>(y);
	viewPort.MaxDepth = 1.0f;
	viewPort.MinDepth = 0.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = static_cast<FLOAT>(x);
}

// シザーのセット
void Union::Scissor(void)
{
	scissor.bottom = static_cast<LONG>(y);
	scissor.left = 0;
	scissor.right = static_cast<LONG>(x);
	scissor.top = 0;
}

// バリアのセット
void Union::Barrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter)
{
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = render->GetResource(swap->Get()->GetCurrentBackBufferIndex());
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	//バリア設置
	list->GetList()->ResourceBarrier(1, &barrier);
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
	list->Reset(pipe->Get());

	list->SetRoot(root->Get());

	list->SetPipe(pipe->Get());

	constant->UpDataWVP();
	constant->SetConstant();

	list->SetViewPort(viewPort);

	list->SetScissor(scissor);

	Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);

	render->SetRender(depth->GetHeap());

	depth->SetDepth();

	constant->UpDataWindow();
	constant->SetConstant(1, 1);
}

// 実行
void Union::Do(void)
{
	Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

	list->Close();

	//コマンド実行.
	ID3D12CommandList* ppCmdLists[] = { list->GetList() };
	queue->Get()->ExecuteCommandLists(_countof(ppCmdLists), ppCmdLists);

	swap->Present();

	fence->Wait();
}

// キー入力
bool Union::CheckKey(UINT index)
{
	return input->CheckKey(index);
}

// トリガーキー入力
bool Union::TriggerKey(UINT index)
{
	return input->TriggerKey(index);
}

// 画像読み込み
void Union::LoadImg(UINT & index, const std::string & fileName)
{
	tex->LoadWIC(index, fileName);
}

// 描画
void Union::Draw(UINT& index, const Vec2f& pos, UINT turnX, UINT turnY)
{
	tex->Draw(index, pos, turnX, turnY);
}

// 描画・サイズ指定
void Union::Draw(UINT & index, const Vec2f & pos, const Vec2f & size, UINT turnX, UINT turnY)
{
	tex->Draw(index, pos, size, turnX, turnY);
}

// 描画・サイズ指定・分割
void Union::Draw(UINT& index, const Vec2f& pos, const Vec2f& size, const Vec2f& rect, const Vec2f& rectSize, UINT turnX, UINT turnY)
{
	tex->Draw(index, pos, size, rect, rectSize, turnX, turnY);
}

// WAVE読み込み
void Union::LoadWave(UINT& index, const std::string & fileName)
{
	audio->LoadWAVE(index, fileName);
}

// WAVEの再生
void Union::PlayWave(UINT& index)
{
	audio->Play(index);
}

// WAVEの再生停止
void Union::StopWave(UINT& index)
{
	audio->Stop(index);
}

// MIDIデバイス数の取得
UINT Union::GetMidiNum(void)
{
	return in->GetDevNum();
}
