#pragma once
#include "../Obj.h"
#include <memory>

class Device;
class List;
class Swap;

class Descriptor :
	public Obj
{
public:
	// コンストラクタ
	Descriptor();
	// デストラクタ
	virtual ~Descriptor();

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}

protected:
	// ヒープの生成
	HRESULT CreateHeap(UINT num, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// ヒープサイズ
	UINT size;
};

