#include "Camerah.h"
#include "../Obj/Obj.h"
#include "../etc/Typedef.h"

// コンストラクタ
Camera::Camera() : 
	pos(0.0f)
{
}

// デストラクタ
Camera::~Camera()
{
}

// ターゲットのセット
void Camera::SetTarget(std::weak_ptr<Obj> obj)
{
	this->obj = obj;
}

// 座標の比較
void Camera::ComparisonPos(float & pos, const float & areaSize, const int & winSize)
{
	auto size = (float)(winSize / 2);

	if (pos - size < 0.0f)
	{
		pos = size;
	}
	else if (pos + size > areaSize)
	{
		pos = areaSize - size;
	}

	if (pos < 0.0f)
	{
		pos = 0.0f;
	}
}

// 処理
void Camera::UpData(const Vec2f & area)
{
	pos = obj.lock()->GetPos();
	ComparisonPos(pos.x, area.x, WINDOW_X);
	ComparisonPos(pos.y, area.y, WINDOW_Y);
}

// ローカル座標に変換
Vec2f Camera::ChangeLocal(const Vec2f & pos)
{
	auto tmp = pos;
	tmp -= { (this->pos.x - (float)(WINDOW_X / 2)), (this->pos.y - (float)(WINDOW_Y / 2)) };

	return tmp;
}
