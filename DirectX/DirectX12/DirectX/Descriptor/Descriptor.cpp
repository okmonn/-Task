#include "Descriptor.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include <tchar.h>

// コンストラクタ
Descriptor::Descriptor() : 
	heap(nullptr), size(0)
{
}

// デストラクタ
Descriptor::~Descriptor()
{
}

// ヒープの生成
HRESULT Descriptor::CreateHeap(UINT num, const D3D12_DESCRIPTOR_HEAP_TYPE & type, const D3D12_DESCRIPTOR_HEAP_FLAGS & flag)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = num;
	desc.Type           = type;

	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nヒープの生成：失敗\n"));
		return result;
	}

	//ヒープサイズの取得
	size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(type);

	return result;
}
