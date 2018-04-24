#include "Player.h"
#include "DxLib.h"
#include "Load.h"

// �R���X�g���N�^
Player::Player()
{

	//�摜�f�[�^�̏�����
	image = LoadGraph(_T("img/rick.png"));
	
	//�t���[�����̏�����
	flam = 0;
	Load::Create();
	Load::GetInstance()->LoadAct("�A�N�V����/player.act");
}

// �f�X�g���N�^
Player::~Player()
{
	Load::Destroy();
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
