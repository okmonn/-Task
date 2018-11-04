#pragma once
#include "../etc/Vector2.h"

class Object
{
public:
	// コンストラクタ
	Object();
	// デストラクタ
	virtual ~Object();

	// 描画
	virtual void Draw(void) = 0;

	// 座標の取得
	Vec2f GetPos(void) const {
		return pos;
	}
	// 座標のセット
	void SetPos(const Vec2f& pos) {
		this->pos = pos;
	}

	// サイズの取得
	Vec2f GetSize(void) const {
		return size;
	}
	// サイズのセット
	void SetSize(const Vec2f& size) {
		this->size = size;
	}

protected:
	// 画像データ
	int image;

	// 座標
	Vec2f pos;

	// サイズ
	Vec2f size;
};
