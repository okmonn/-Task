#pragma once
#include "../etc/VertexData.h"
#include <vector>
#include <memory>

struct ID3D12Resource;

class Union;
class Device;
class List;
class Constant;
class Root;
class Pipe;

class Point
{
public:
	// コンストラクタ
	Point(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Point();

	// 頂点の追加
	void AddVertexPoint(const float & x, const float & y, const float & r, const float & g, const float & b, const float& alpha);
	
	// 描画
	void Draw(void);

	// 頂点のリセット
	void Reset(void);

private:
	// リソースの生成
	long Create(void);

	// マップ
	long Map(void);


	// ユニオン
	Union& un;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// 定数バッファ
	std::weak_ptr<Constant>con;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// リソース
	ID3D12Resource* rsc;

	// 送信データ
	unsigned int* data;

	// 頂点データ
	std::vector<draw::Vertex>vertex;
};
