#include "Camerah.h"
#include "../Object/Object.h"
#include "../etc/Typedef.h"

// �R���X�g���N�^
Camera::Camera() : 
	pos(0.0f), large(3.0f)
{
}

// �f�X�g���N�^
Camera::~Camera()
{
}

// �^�[�Q�b�g�̃Z�b�g
void Camera::SetTarget(std::weak_ptr<Object> obj)
{
	this->obj = obj;
}

// ���W�̔�r
void Camera::ComparisonPos(float & pos, const float & areaSize, const int & winSize)
{
	auto size = (float)(winSize / 2);

	if (pos - size < 0.0f)
	{
		pos = size;
	}
	else if (pos + size > areaSize * large)
	{
		pos = (areaSize * large) - size;
	}

	if (pos < 0.0f)
	{
		pos = 0.0f;
	}
}

// ����
void Camera::UpData(const Vec2f & area)
{
	pos = obj.lock()->GetPos() + (obj.lock()->GetSize() / 2.0f);
	ComparisonPos(pos.x, area.x, WINDOW_X);
	ComparisonPos(pos.y, area.y, WINDOW_Y);
}

// ���[�J�����W�ɕϊ�
Vec2f Camera::ChangeLocal(const Vec2f & pos)
{
	auto tmp = pos;
	tmp -= { (this->pos.x - (float)(WINDOW_X / 2)), (this->pos.y - (float)(WINDOW_Y / 2)) };

	return tmp;
}