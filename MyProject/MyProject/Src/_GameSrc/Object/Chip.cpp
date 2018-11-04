#include "Chip.h"
#include "../Camera/Camerah.h"
#include "../etc/Typedef.h"
#include "../../Func/Func.h"

// コンストラクタ
Chip::Chip(const std::string & fileName, const int & id, std::weak_ptr<Camera>cam, const Vec2f & rect, const Vec2f & pos, const Vec2f & size) :
	cam(cam), rect(rect)
{
	image = id;
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

	if (0.0f <= pos.x && pos.x + size.x <= WINDOW_X
		&& 0.0f <= pos.y && pos.y + size.y <= WINDOW_Y)
	{
		func::DrawRectImg(image, pos.x, pos.y, size.x, size.y, rect.x, rect.y, size.x, size.y);
	}
}
