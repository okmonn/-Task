#include "Player.h"
#include "../Camera/Camerah.h"
#include "../../Func/Func.h"



// �R���X�g���N�^
Player::Player(std::weak_ptr<Camera>cam)
{
	this->cam = cam;
	func::LoadImg("Rsc/Player.png", image);
	size = { 64, 64 };
}

// �f�X�g���N�^
Player::~Player()
{
	func::DeleteImg(image);
}

// �`��
void Player::Draw(void)
{
	lpos = cam.lock()->ChangeLocal(pos);
	func::DrawRectImg(image, lpos.x, lpos.y, size.x, size.y, 0, 0, 100, 59);
}

// ����
void Player::UpData(void)
{
	if (func::CheckKey(INPUT_RIGHT))
	{
		pos.x += speed;
	}
	else if (func::CheckKey(INPUT_LEFT))
	{
		pos.x -= speed;
	}
}
