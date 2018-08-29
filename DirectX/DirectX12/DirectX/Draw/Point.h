#pragma once
#include "../Descriptor/Descriptor.h"
#include "../../etc/Typedef.h"
#include "../../etc/Vector2.h"
#include "../../etc/Vector3.h"
#include <map>

class Point :
	public Descriptor
{
public:
	// コンストラクタ
	Point(std::weak_ptr<Device>dev, std::weak_ptr<List>list, const Vec2f& pos, const Vec3f& color);
	// デストラクタ
	~Point();

	// 描画
	HRESULT Draw(void);

private:
	// リソースの生成
	HRESULT CreateResource(void);


	// 頂点データ
	Vertex vertex;

	// リソース
	ID3D12Resource * resource;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW view;

	//送信データ
	UINT8* data;

	// 座標
	Vec2f pos;

	// 色
	Vec3f color;
};

