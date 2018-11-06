#pragma once
#include "Object.h"
#include <string>

class Chip :
	public Object
{
public:
	// コンストラクタ
	Chip(const std::string& fileName, const int& id, std::weak_ptr<Camera>cam, const Vec2f& rectPos, const Vec2f& rectSize, const Vec2f& pos, const Vec2f& size);
	// デストラクタ
	~Chip();

	// 描画
	void Draw(void);

private:

};
