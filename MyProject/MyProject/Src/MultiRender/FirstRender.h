#pragma once
#include "MultiRender.h"
#include <vector>

class FirstRender :
	public MultiRender
{
public:
	// コンストラクタ
	FirstRender(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~FirstRender();

	// 描画
	void Draw(void);

private:
	// 初期化
	void Init(void);

	// レンダーターゲットビューの生成
	void CreateRtv(void);

	// シェーダーリソースビューの生成
	void CreateSrv(void);

	// 頂点リソースの生成
	long CreateVertex(void);

	// 頂点のマップ
	long Map(void);


	// 頂点リソース
	ID3D12Resource* vertex;

	// 頂点
	std::vector<multi::Vertex>v;

	// 送信データ
	unsigned int* data;
};
