#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;
class List;
class Depth;
class Constant;
class Root;
class Pipe;

class Primitive
{
	struct VERTEX
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

public:
	// コンストラクタ
	Primitive();
	// デストラクタ
	virtual ~Primitive();

	// 描画
	void Draw(void);

protected:
	// 頂点セット
	void SetVertex(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& size);

	// リソースの生成
	long CreateRsc(void);

	// マップ
	long Map(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// リスト
	std::weak_ptr<List>list;

	// 定数
	std::weak_ptr<Constant>con;

	// 深度バッファ
	std::weak_ptr<Depth>depth;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// リソース
	ID3D12Resource* rsc;

	//頂点情報
	std::vector<VERTEX>vertex;

	// 送信データ
	void* data;
};
