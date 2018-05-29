#include "BackGround.h"
#include "Typedef.h"
#include <tchar.h>

#define BACK_SIZE_X 576

// �R���X�g���N�^
BackGround::BackGround(std::weak_ptr<Camera>cam) : cam(cam)
{
	//�w�i
	back = LoadGraph(_T("img/splatterhouse.png"));

	//���W
	pos = { 0,0 };
	pos.x = this->cam.lock()->GetPos().x;
	z = this->cam.lock()->GetPos().x;;
}

// �f�X�g���N�^
BackGround::~BackGround()
{
	DeleteGraph(back);
}

// ���W�̃Z�b�g
void BackGround::SetPos(Positionf pos)
{
	this->pos.x = pos.x;
}

// �`��
void BackGround::Draw(void)
{
	//�w�i
	DrawGraph(-((int)pos.x % (BACK_SIZE_X * 2)), (int)pos.y, back, false);
	DrawTurnGraph(-((int)pos.x % (BACK_SIZE_X * 2)) + BACK_SIZE_X, (int)pos.y, back, false);
	DrawGraph(-((int)pos.x % (BACK_SIZE_X * 2)) + BACK_SIZE_X * 2, (int)pos.y, back, false);
	DrawTurnGraph(-((int)pos.x % (BACK_SIZE_X * 2)) + BACK_SIZE_X * 3, (int)pos.y, back, false);
}

// ����
void BackGround::UpData(void)
{
	pos.x = cam.lock()->GetPos().x;
}
