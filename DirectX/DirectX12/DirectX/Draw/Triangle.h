#pragma once
#include "../Descriptor/Descriptor.h"
#include "../../etc/Typedef.h"
#include <vector>

class Triangle :
	public Descriptor
{
public:
	// コンストラクタ
	Triangle(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~Triangle();

	// 描画
	void Draw(void);

	// 頂点リソースの取得
	ID3D12Resource* GetResource(void) const {
		return resource;
	}

private:
	// リソースの生成
	HRESULT CreateResource(void);


	// 頂点
	std::vector<Vertex>vertex;

	// リソース
	ID3D12Resource * resource;

	// 送信データ
	UINT8* data;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW view;
};

