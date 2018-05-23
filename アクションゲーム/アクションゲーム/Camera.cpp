#include "Camera.h"
#include "Player.h"
#include "DxLib.h"
#include <cassert>

// �R���X�g���N�^
Camera::Camera(std::weak_ptr<Stage>st) : st(st)
{
}

// �f�X�g���N�^
Camera::~Camera()
{
}
// ���S�I�u�W�F�N�g�̐ݒ�
void Camera::SetFocus(std::shared_ptr<Player>pl)
{
	this->pl = pl;
}

// ���W�̎擾
Positionf & Camera::GetPos(void)
{
	return pos;
}

// �ڎ��T�C�Y�̎擾
Rect & Camera::GetViewSize(void)
{
	return st.lock()->GetStageRange();
}

// �`��
void Camera::Draw(void)
{
	DrawFormatString(620, 50, GetColor(0, 255, 0), "%d", (int)pos.x);
	DrawFormatString(650, 50, GetColor(0, 255, 0), "%d", (int)pos.y);
}

// ����
void Camera::UpData(void)
{
	assert(!pl.expired());
	auto& range = st.lock()->GetStageRange();
	
	pos = pl.lock()->GetPos();
	//�J�������W�̕␳ 
	if (pos.x - WINDOW_X / 2 < range.GetLeft()) 
	{
		pos.x = (float)(range.GetLeft() + WINDOW_X / 2);
	}
	else if (pos.x + WINDOW_X / 2 > range.GetRight()) 
	{
		pos.x = (float)(range.GetRight() - WINDOW_X / 2);
	}
}

// ���W�C��
Positionf Camera::CorrectionPos(Positionf pos)
{
	//���W����J�����̍��W������
	Positionf tmp = pos;
	
	if (tmp.x > WINDOW_X / 2)
	{
		tmp.x -= this->pos.x - WINDOW_X / 2;
	}

	return tmp;
}
