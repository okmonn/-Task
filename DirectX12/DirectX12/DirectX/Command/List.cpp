#include "List.h"
#include "../Device.h"
#include "Allocator.h"

// コンストラクタ
List::List(std::weak_ptr<Device>dev, std::weak_ptr<Allocator>allo) : dev(dev), allo(allo), list(nullptr)
{
	Create();
}

// デストラクタ
List::~List()
{
	Release(list);
}

// コマンドリストの生成
HRESULT List::Create(void)
{
	result = dev.lock()->Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, allo.lock()->Get(), nullptr, IID_PPV_ARGS(&list));

	OutDebug(L"\nコマンドリストの生成：失敗\n", result);

	return result;
}
