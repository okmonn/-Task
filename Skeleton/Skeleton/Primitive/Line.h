#pragma once
#include "Primitive.h"

class Line :
	public Primitive
{
public:
	// コンストラクタ
	Line(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Line();

	// 頂点のセット
	long SetVertex(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2, const DirectX::XMFLOAT3& color, const float& alpha);

	// 描画
	void Draw(std::weak_ptr<List>list);

private:
	// 初期化
	void Init(void);

	// バンドルのセット
	void Bundle(void);
};
