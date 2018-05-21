#include "Title.h"
#include "Game.h"
#include "Play.h"
#include "DxLib.h"

// �R���X�g���N�^
Title::Title(std::weak_ptr<Input>in)
{
	//�C���v�b�g�N���X
	this->in = in;
}

// �f�X�g���N�^
Title::~Title()
{
	DrawString(300, 300, "Title Scene END", 0xffffff);
}

// �`��
void Title::Draw(void)
{
	DrawString(300, 300, "Title Scene", 0xffffff);
}

// ����
void Title::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Play(in));
	}
}
