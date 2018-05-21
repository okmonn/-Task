#include "Interface.h"
#include "Typedef.h"
#include "DxLib.h"

#define BAR_SIZE_Y  64


// �R���X�g���N�^
Interface::Interface()
{
	//�㕔�o�[
	top = LoadGraph(_T("img/bar_top.png"));

	//�����o�[
	bottom = LoadGraph(_T("img/bar_bottom.png"));
}

// �f�X�g���N�^
Interface::~Interface()
{
	DeleteGraph(top);
	DeleteGraph(bottom);
}

// �`��
void Interface::Draw(void)
{
	//�㕔�o�[
	DrawGraph(0, 0, top, false);

	//�����o�[
	DrawGraph(0, (WINDOW_Y - BAR_SIZE_Y), bottom, false);
}

// ����
void Interface::UpData(void)
{
}
