#include "Title.h"
#include "Game.h"
#include "Play.h"
#include "DxLib.h"

// �R���X�g���N�^
Title::Title(std::weak_ptr<Input>in)
{
	//�C���v�b�g�N���X
	this->in = in;

	//�摜�n���h��
	image = LoadGraph("img/title.png");
}

// �f�X�g���N�^
Title::~Title()
{
	DeleteGraph(image);
}

// �`��
void Title::Draw(void)
{
	DrawGraph(0, 0, image, true);

	DrawString(300, 400, "PUSH START (W)", GetColor(255, 0, 0), false);
}

// ����
void Title::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Play(in));
	}
}
