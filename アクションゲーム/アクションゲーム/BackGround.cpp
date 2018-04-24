#include "BackGround.h"
#include "Typedef.h"
#include "DxLib.h"
#include <tchar.h>

#define BACK_SIZE_X 576
#define BAR_SIZE_Y  64

// �R���X�g���N�^
BackGround::BackGround()
{
	//�w�i
	back = LoadGraph(_T("�A�N�V����/img/splatterhouse.png"));

	//�㕔�o�[
	top = LoadGraph(_T("�A�N�V����/img/bar_top.png"));

	//�����o�[
	bottom = LoadGraph(_T("�A�N�V����/img/bar_bottom.png"));
}

// �f�X�g���N�^
BackGround::~BackGround()
{
}

// �`��
void BackGround::Draw(void)
{
	//�w�i
	DrawGraph(0, 0, back, false);
	DrawTurnGraph(BACK_SIZE_X, 0, back, false);

	//�㕔�o�[
	DrawGraph(0, 0, top, false);

	//�����o�[
	DrawGraph(0, (WINDOW_Y - BAR_SIZE_Y), bottom, false);
}

// ����
void BackGround::UpData(void)
{
}
