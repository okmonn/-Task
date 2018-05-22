#include "Over.h"
#include "Game.h"
#include "Title.h"
#include "DxLib.h"

// �R���X�g���N�^
Over::Over(std::weak_ptr<Input>in)
{
	//�C���v�b�g�N���X
	this->in = in;
	
	//�摜
	image = LoadGraph("img/gameover.png");
}

// �f�X�g���N�^
Over::~Over()
{
	DeleteGraph(image);
}

// �`��
void Over::Draw(void)
{
	DrawGraph(0, 0, image, true);
}

// ����
void Over::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Title(in));
	}
}
