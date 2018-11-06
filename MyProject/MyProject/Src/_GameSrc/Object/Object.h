#pragma once
#include "../etc/Vector2.h"
#include <memory>

class Camera;

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

	// 中心座標の取得
	Vec2f GetCenter(void) const {
		return { pos.x + size.x / 2.0f, pos.y + size.y / 2.0f };
	}

protected:
	// カメラ
	std::weak_ptr<Camera>cam;

	// 画像データ
	int image;

	// 座標
	Vec2f pos;

	// サイズ
	Vec2f size;

	// 切り取り座標
	Vec2f rectPos;

	// 切り取りサイズ
	Vec2f rectSize;
};
