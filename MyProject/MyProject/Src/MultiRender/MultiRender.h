#pragma once
#include <memory>

enum D3D12_DESCRIPTOR_HEAP_TYPE : int;

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;
class List;
class Swap;
class Render;
class Root;
class Pipe;

class MultiRender
{
public:
	// コンストラクタ
	MultiRender();
	// デストラクタ
	virtual ~MultiRender();

	// レンダーターゲットのセット
	void SetRender(ID3D12DescriptorHeap& depth, const float* color);

	// レンダーターゲット用ヒープの取得
	ID3D12DescriptorHeap* GetRtv(void) const {
		return rtv;
	}
	// シェーダー用ヒープの取得
	ID3D12DescriptorHeap* GetSrv(void) const {
		return srv;
	}
	// リソースの取得
	ID3D12Resource* GetRsc(void) const {
		return rsc;
	}

protected:
	// ヒープの生成
	long CreateHeap(ID3D12DescriptorHeap** heap, const D3D12_DESCRIPTOR_HEAP_TYPE& type);

	// リソースの生成
	long CreateRsc(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドりリスト
	std::weak_ptr<List>list;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// レンダーターゲット
	std::weak_ptr<Render>render;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// レンダーターゲット用ヒープ
	ID3D12DescriptorHeap* rtv;

	// シェーダー用ヒープ
	ID3D12DescriptorHeap* srv;

	// リソース
	ID3D12Resource* rsc;
};
