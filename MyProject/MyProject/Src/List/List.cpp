#include "List.h"
#include "../Union/Union.h"
#include "../Device/Device.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../MultiRender/MultiRender.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// コンストラクタ
List::List(std::weak_ptr<Device>dev) : 
	un(Union::Get()), dev(dev), allo(nullptr), list(nullptr)
{
	CreateAllo();
	CreateList();
}

// デストラクタ
List::~List()
{
	Release(list);
	Release(allo);
}

// コマンドアロケータの生成
long List::CreateAllo(void)
{
	auto hr = dev.lock()->Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコマンドアロケータの生成：失敗\n"));
	}

	return hr;
}

// コマンドリストの生成
long List::CreateList(void)
{
	auto hr = dev.lock()->Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコマンドリストの生成：失敗\n"));
		return hr;
	}

	Close();

	return hr;
}

// リセット
void List::Reset(ID3D12PipelineState * pipe)
{
	allo->Reset();
	list->Reset(allo, pipe);
}

// ルートシグネチャのセット
void List::SetRoot(ID3D12RootSignature & root)
{
	list->SetGraphicsRootSignature(&root);
}

// パイプラインのセット
void List::SetPipe(ID3D12PipelineState & pipe)
{
	list->SetPipelineState(&pipe);
}

// ビューポートのセット
void List::SetViewport(void)
{
	//ビューポート設定用構造体
	D3D12_VIEWPORT view = {};
	view.Height   = static_cast<float>(un.GetWinY());
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width    = static_cast<float>(un.GetWinX());

	list->RSSetViewports(1, &view);
}

void List::SetViewport(const unsigned & w, const unsigned & h)
{
	D3D12_VIEWPORT view = {};
	view.Height = static_cast<float>(h);
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width = static_cast<float>(w);

	list->RSSetViewports(1, &view);
}

// シザーのセット
void List::SetScissor(void)
{
	//シザー設定用構造体
	RECT scissor = {};
	scissor.bottom = static_cast<LONG>(un.GetWinY());
	scissor.left   = 0;
	scissor.right  = static_cast<LONG>(un.GetWinX());
	scissor.top    = 0;

	list->RSSetScissorRects(1, &scissor);
}

void List::SetScissor(const unsigned & w, const unsigned & h)
{
	RECT scissor = {};
	scissor.bottom = static_cast<LONG>(h);
	scissor.left = 0;
	scissor.right = static_cast<LONG>(w);
	scissor.top = 0;

	list->RSSetScissorRects(1, &scissor);
}

// バリアのセット
void List::SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, std::weak_ptr<Swap>swap, std::weak_ptr<Render>render)
{
	//バリアの設定
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = render.lock()->GetRsc(swap.lock()->Get()->GetCurrentBackBufferIndex());
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}

// バリアのセット
void List::SetBarrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, std::weak_ptr<MultiRender> multi)
{
	//バリアの設定
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = multi.lock()->GetRsc();
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}

// バリアのセット
void List::SetBarrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc)
{
	//バリアの設定
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = rsc;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}

// コマンドを閉じる
void List::Close(void)
{
	list->Close();
}
