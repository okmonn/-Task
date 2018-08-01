#include "List.h"
#include "../Device.h"
#include <tchar.h>

// コンストラクタ
List::List(std::weak_ptr<Device>dev) : allo(nullptr), list(nullptr)
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
	}

	list->Close();

	return result;
}
