#include "Over.h"
#include "Game.h"
#include "Title.h"
#include "DxLib.h"

const int time = 300;

// �R���X�g���N�^
Over::Over(std::weak_ptr<Input>in)
{
	//�C���v�b�g�N���X
	this->in = in;
	
	//�摜
	image = LoadGraph("img/gameover.png");

	flam = 0;
}

// �f�X�g���N�^
Over::~Over()
{
	DeleteGraph(image);
}

// �`��
void Over::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, time - flam);
	DrawGraph(0, 0, image, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// ����
void Over::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8)
		|| flam >= time)
	{
		Game::Instance().ChangeScene(new Title(in));
	}
	else
	{
		++flam;
	}
}
