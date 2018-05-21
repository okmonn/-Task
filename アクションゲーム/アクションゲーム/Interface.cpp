#include "Interface.h"
#include "Typedef.h"
#include "DxLib.h"

#define BAR_SIZE_Y  64


// �R���X�g���N�^
Interface::Interface(std::weak_ptr<Player>pl) : pl(pl)
{
	//�㕔�o�[
	top = LoadGraph(_T("img/bar_top.png"));

	//�����o�[
	bottom = LoadGraph(_T("img/bar_bottom.png"));

	//�̗�
	hp = LoadGraph("img/heart.png");
}

// �f�X�g���N�^
Interface::~Interface()
{
	DeleteGraph(top);
	DeleteGraph(bottom);
	DeleteGraph(hp);
}

// �`��
void Interface::Draw(void)
{
	//�㕔�o�[
	DrawGraph(0, 0, top, true);

	//�����o�[
	DrawGraph(0, (WINDOW_Y - BAR_SIZE_Y), bottom, true);

	//�̗�
	for (int i = 0; i < 3; ++i)
	{
		DrawGraph(220 + (i * 50), (WINDOW_Y - 45), hp, true);
	}
}

// ����
void Interface::UpData(void)
{
}
