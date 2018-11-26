#pragma once
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Union;
class Device;
class List;

class Depth
{
public:
	// コンストラクタ
	Depth(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~Depth();

	// 深度ステンシルのセット
	void SetDepth(void);

	void SetShadow(void);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	ID3D12DescriptorHeap* GetSrvHeap(void) const {
		return srv;
	}
	// リソースの取得
	ID3D12Resource* GetRsc(void) const {
		return rsc;
	}
	ID3D12DescriptorHeap* GetShadowHeap(void) const {
		return shadowHeap;
	}
	ID3D12Resource* GetShadowRsc(void) const {
		return shadowRsc;
	}

private:
	// ヒープの生成
	long CreateHeap(void);

	// SRVヒープの生成
	long CreateSrvHeap(void);

	// シャドウヒープの生成
	long CreateShadowHeap(void);

	// リソースの生成
	long CreateRsc(void);

	// シャドウリソースの生成
	long CreateShadowRsc(void);

	// リソースビューの生成
	void CreateView(void);

	// シャドウリソースビューの生成
	void CreateShadowView(void);

	// SRVビューの生成
	void CreateSrvView(void);

	// シャドウSRVの生成
	void CreateShadowSrv(void);

	// 初期化
	void Init(void);


	// ユニオン
	Union& un;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// SRVヒープ
	ID3D12DescriptorHeap* srv;

	// リソース
	ID3D12Resource* rsc;

	// シャドウヒープ
	ID3D12DescriptorHeap* shadowHeap;

	// シャドウリソース
	ID3D12Resource* shadowRsc;
};
