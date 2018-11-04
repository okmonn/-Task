#pragma once
#include "Object.h"
#include <string>
#include <memory>

class Camera;

class Chip :
	public Object
{
public:
	// コンストラクタ
	Chip(const std::string& fileName, const int& id, std::weak_ptr<Camera>cam, const Vec2f& rect = 0.0f, const Vec2f& pos = 0.0f, const Vec2f& size = 0.0f);
	// デストラクタ
	~Chip();

	// 描画
	void Draw(void);

	// 画像の切り取り矩形のセット
	void SetRect(const float& x, const float& y) {
		rect = { x, y };
	}

private:
	// カメラ
	std::weak_ptr<Camera>cam;

	// 切り取り矩形
	Vec2f rect;
};
