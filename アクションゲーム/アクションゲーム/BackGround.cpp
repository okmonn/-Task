#include "BackGround.h"
#include <tchar.h>

#define BACK_SIZE_X 576

// �R���X�g���N�^
BackGround::BackGround()
{
	//�w�i
	back = LoadGraph(_T("img/splatterhouse.png"));
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
}

// ����
void BackGround::UpData(void)
{
}
