#include "Title.h"
#include "../../Func/Func.h"

int n = 0;

// �R���X�g���N�^
Title::Title()
{
	func::LoadImg("avicii.png", n);
}

// �f�X�g���N�^
Title::~Title()
{
}

// �`��
void Title::Draw(void)
{
	func::DrawImg(n, 0, 0);
}

// ����
void Title::UpData(void)
{
}
