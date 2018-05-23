#include "Continue.h"
#include "Game.h"
#include "Play.h"
#include "Over.h"
#include "DxLib.h"

#define COUNT_X 5
#define CONUT_Y 2

#define COUNT_MAX 10

#define COUNT_SIZE_X 117
#define COUNT_SIZE_Y 159

const int time = 90;

// �R���X�g���N�^
Continue::Continue(std::weak_ptr<Input>in) : flam(0)
{
	//�C���v�b�g�N���X
	this->in = in;

	//�摜
	image = LoadGraph("img/count.png");
}

// �f�X�g���N�^
Continue::~Continue()
{
	DeleteGraph(image);
}

// �`��
void Continue::Draw(void)
{
	DrawRectGraph(300, 150, 
		(COUNT_SIZE_X * ((flam++ / time) % COUNT_X)),
		(COUNT_SIZE_Y * ((flam++ / (time * COUNT_X)) % CONUT_Y)),
		COUNT_SIZE_X, COUNT_SIZE_Y, image, true);
}

// ����
void Continue::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_1) || in.lock()->CheckTrigger(PAD_INPUT_2))
	{
		flam += time - (flam % time);
	}
	if (flam < time * COUNT_MAX)
	{
		if (in.lock()->CheckTrigger(PAD_INPUT_8))
		{
			Game::Instance().ChangeScene(new Play(in));
		}
	}
	else
	{
		Game::Instance().ChangeScene(new Over(in));
	}
}
