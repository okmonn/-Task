#pragma once
#include "../Obj.h"
#include <vector>
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
	ID3D12DescriptorHeap* GetHeap(UINT i = 0) const {
		return heap[i];
	}
	// ヒープの取得
	std::vector<ID3D12DescriptorHeap*> GetVecHeap(void) const {
		return heap;
	}

	// リソースの取得
	ID3D12Resource* GetResource(UINT i = 0) const {
		return resource[i];
	}
	// リソースの取得
	std::vector<ID3D12Resource*> GetVecResource(void) const {
		return resource;
	}

	// ヒープサイズの取得
	UINT GetSIze(void) const {
		return size;
	}

	// リソースの解放
	void ResourceLiberation(UINT i, bool unmap = false);
	// ヒープの解放
	void HeapLiberation(UINT i);

protected:
	// ヒープの生成
	HRESULT CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	// リセット
	void Reset(void);

	// 解放
	void Liberation(bool unmap = false);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// ヒープ
	std::vector<ID3D12DescriptorHeap*>heap;

	// リソース
	std::vector<ID3D12Resource*>resource;

	// ヒープサイズ
	UINT size;
};

