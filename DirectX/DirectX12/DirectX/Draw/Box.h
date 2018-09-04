#pragma once
#include "../Descriptor/Descriptor.h"
#include "../../etc/Typedef.h"
#include "../../etc/Vector2.h"
#include "../../etc/Vector3.h"
#include <vector>
#include <map>

class Pipe;

class Box :
	public Descriptor
{
public:
	// コンストラクタ
	Box(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe, UINT max = 4 * 100);
	// デストラクタ
	~Box();

	// 頂点データの追加
	void AddList(const Vec2f& pos, const Vec2f& size, const Vec3f& color, float alpha = 1.0f);

	// 描画
	void Draw(void);

	// 配列のリセット
	void Reset(void);

private:
	// リソースの生成
	HRESULT CreateResource(void);


	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// リソース
	ID3D12Resource * resource;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW view;

	//送信データ
	UINT8* data;

	// 頂点データ
	std::vector<Vertex>vertex;

	// 頂点最大数
	UINT vertexMax;
};

