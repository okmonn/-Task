#include "List.h"
#include "../../Device/Device.h"
#include <tchar.h>

// コンストラクタ
List::List(std::weak_ptr<Device>dev) : 
	allo(nullptr), list(nullptr)
{
	this->dev = dev;

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
HRESULT List::CreateAllo(void)
{
	result = dev.lock()->Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allo));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nコマンドアロケータの生成：失敗\n"));
	}

	return result;
}

// コマンドリストの生成
HRESULT List::CreateList(void)
{
	result = dev.lock()->Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nコマンドリストの生成：失敗\n"));
		return result;
	}

	Close();

	return result;
}

// リセット
void List::Reset(ID3D12PipelineState * pipe)
{
	//コマンドアロケータのリセット
	allo->Reset();

	//コマンドリストのリセット
	list->Reset(allo, pipe);
}

// ルートシグネチャのセット
void List::SetRoot(ID3D12RootSignature * root)
{
	list->SetGraphicsRootSignature(root);
}

// パイプラインのセット
void List::SetPipe(ID3D12PipelineState * pipe)
{
	list->SetPipelineState(pipe);
}

// ビューポートのセット
void List::SetViewPort(const D3D12_VIEWPORT & viewport)
{
	list->RSSetViewports(1, &viewport);
}

// シザーのセット
void List::SetScissor(const RECT & scissor)
{
	list->RSSetScissorRects(1, &scissor);
}

// コマンドリストのクローズ
void List::Close(void)
{
	list->Close();
}
