#include "Player.h"
#include "DxLib.h"
#include "Typedef.h"

// �R���X�g���N�^
Player::Player()
{
	//�摜�f�[�^�̏�����
	image = LoadGraph("img/rick.png");
	
	//�t���[�����̏�����
	flam = 0;
}

// �f�X�g���N�^
Player::~Player()
{
}

// �`��
void Player::Draw(void)
{
	DrawRotaGraph2(0, 0, 50, 50, 2.0f, 0.0f, image, true);
	
	++flam;
}

// ����
void Player::UpData(void)
{
}
