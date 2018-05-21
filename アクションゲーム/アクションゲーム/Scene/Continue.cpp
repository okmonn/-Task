#include "Continue.h"
#include "Game.h"
#include "Play.h"
#include "Over.h"
#include "DxLib.h"

// �R���X�g���N�^
Continue::Continue(std::weak_ptr<Input>in)
{
	//�C���v�b�g�N���X
	this->in = in;
}

// �f�X�g���N�^
Continue::~Continue()
{
	DrawString(300, 300, "Continue Scene END", 0xffffff);
}

// �`��
void Continue::Draw(void)
{
	DrawString(300, 300, "Continue Scene", 0xffffff);
}

// ����
void Continue::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Over(in));
	}
	else if (in.lock()->CheckTrigger(PAD_INPUT_9))
	{
		Game::Instance().ChangeScene(new Play(in));
	}
}
