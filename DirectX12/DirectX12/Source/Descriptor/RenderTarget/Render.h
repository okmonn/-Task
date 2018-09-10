#pragma once
#include "../Descriptor.h"
#include <vector>

class Swap;

class Render :
	public Descriptor
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

	// レンダーターゲットのセット
	void SetRender(ID3D12DescriptorHeap* depth);

	// リソースの取得
	ID3D12Resource* GetResource(UINT index) const {
		return resource[index];
	}
	// リソースの取得
	std::vector<ID3D12Resource*>GetResource(void) const {
		return resource;
	}

private:
	// リソースの生成
	HRESULT CreateResource(void);


	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// リソース
	std::vector<ID3D12Resource*>resource;
};

