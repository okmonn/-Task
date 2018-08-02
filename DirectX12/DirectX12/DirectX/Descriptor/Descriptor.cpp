#include "Descriptor.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include <tchar.h>

// コンストラクタ
Descriptor::Descriptor() : size(0)
{
	Reset();
}

// デストラクタ
Descriptor::~Descriptor()
{
}

// ヒープの生成
HRESULT Descriptor::CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag)
{
	//ダミー宣言
	ID3D12DescriptorHeap* dummy = nullptr;

	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = swap.lock()->GetBack();
	desc.Type           = type;

	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&dummy));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nヒープの生成：失敗\n"));
	}

	//ヒープの追加
	heap.push_back(dummy);

	//ヒープサイズの更新
	size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(type);

	//ダミーの解放
	//Release(dummy);

	return result;
}

// リセット
void Descriptor::Reset(void)
{
	heap.clear();
	resource.clear();
}

// リソースの解放
void Descriptor::ResourceLiberation(UINT i, bool unmap)
{
	if (unmap == true)
	{
		resource[i]->Unmap(0, nullptr);
	}
	Release(resource[i]);
}

// ヒープの解放
void Descriptor::HeapLiberation(UINT i)
{
	Release(heap[i]);
}

// 解放
void Descriptor::Liberation(bool unmap)
{
	for (UINT i = 0; i < resource.size(); ++i)
	{
		if (unmap == true)
		{
			resource[i]->Unmap(0, nullptr);
		}
		Release(resource[i]);
	}

	for (UINT i = 0; i < heap.size(); ++i)
	{
		Release(heap[i]);
	}
}
