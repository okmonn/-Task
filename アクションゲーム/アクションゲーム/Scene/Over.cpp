#include "Over.h"
#include "Game.h"
#include "Title.h"
#include "DxLib.h"

// �R���X�g���N�^
Over::Over(std::weak_ptr<Input>in)
{
	//�C���v�b�g�N���X
	this->in = in;
}

// �f�X�g���N�^
Over::~Over()
{
	DrawString(300, 300, "Over Scene END", 0xffffff);
}

// �`��
void Over::Draw(void)
{
	DrawString(300, 300, "Over Scene", 0xffffff);
}

// ����
void Over::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Title(in));
	}
}
