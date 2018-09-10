#pragma once
#include "../BaseObj/Obj.h"
#include <memory>

class Device;
class List;

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

	// ヒープサイズの取得
	UINT GetSize(void) const {
		return size;
	}

protected:
	// ヒープの生成
	HRESULT CreateHeap(UINT num, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// ヒープサイズ
	UINT size;
};

