#include "Chip.h"
#include "../Camera/Camerah.h"
#include "../../Func/Func.h"

Chip::Chip(const std::string & fileName, const int & id, std::weak_ptr<Camera> cam, const Vec2f & rectPos, const Vec2f & rectSize, const Vec2f & pos, const Vec2f & size)
{
	this->cam = cam;

	image = id;
	this->rectPos = rectPos;
	this->rectSize = rectSize;
	this->pos = pos;
	this->size = size;

	func::LoadImg(fileName, image);
}

// デストラクタ
Chip::~Chip()
{
	func::DeleteImg(image);
}

// 描画
void Chip::Draw(void)
{
	auto pos = cam.lock()->ChangeLocal(this->pos);

	if (0.0f <= pos.x && pos.x + size.x <= func::GetWinSizeX()
		&& 0.0f <= pos.y && pos.y + size.y <= func::GetWinSizeY())
	{
		func::DrawRectImg(image, pos.x, pos.y, size.x, size.y, rectPos.x, rectPos.y, rectSize.x, rectSize.y);
	}
}
