#pragma once
#include <memory>
#include <DirectXMath.h>

enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_DESCRIPTOR_HEAP_FLAGS : int;

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;
class List;
class Render;
class Root;
class Pipe;

namespace multi {
	// 頂点
	struct Vertex {
		//座標
		DirectX::XMFLOAT3 pos;
		//uv
		DirectX::XMFLOAT2 uv;
	};
}

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
	long CreateHeap(ID3D12DescriptorHeap** heap, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	// リソースの生成
	long CreateRsc(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドりリスト
	std::weak_ptr<List>list;

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
