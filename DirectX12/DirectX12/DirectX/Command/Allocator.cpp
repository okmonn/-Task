#include "Allocator.h"
#include "../Device.h"

// コンストラクタ
Allocator::Allocator(std::weak_ptr<Device>dev) : dev(dev), allo(nullptr)
{
}

// デストラクタ
Allocator::~Allocator()
{
	Release(allo);
}

// コマンドアロケータの生成
HRESULT Allocator::Create(void)
{
	result = dev.lock()->Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allo));
	
	OutDebug(L"\nコマンドアロケータの生成：失敗\n", result);

	return result;
}
