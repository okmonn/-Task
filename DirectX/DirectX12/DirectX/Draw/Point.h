#pragma once
#include "../Descriptor/Descriptor.h"
#include "../../etc/Typedef.h"
#include "../../etc/Vector2.h"
#include "../../etc/Vector3.h"
#include <vector>
#include <map>

class Window;
class Root;
class Pipe;
class Constant;

class Point :
	public Descriptor
{
public:
	// コンストラクタ
	Point(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<Constant>con);
	// デストラクタ
	~Point();

	// 頂点データの追加
	void AddList(const Vec2f& pos, const Vec3f& color, float alpha = 1.0f);

	// 描画
	void Draw(void);

	// 配列のリセット
	void Reset(void);

private:
	// リソースの生成
	HRESULT CreateResource(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// 定数バッファ
	std::weak_ptr<Constant>con;

	// リソース
	ID3D12Resource * resource;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW view;

	//送信データ
	UINT8* data;

	// 頂点データ
	std::vector<PointVertex>vertex;

	// 頂点最大数
	UINT vertexMax;
};

