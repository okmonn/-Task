#include "Title.h"
#include "../../Func/Func.h"

int n = 0;

// �R���X�g���N�^
Title::Title()
{
	func::LoadPmd("Model/�㉹�n�N.pmd", n);
	func::Attach("charge.vmd", n);
}

// �f�X�g���N�^
Title::~Title()
{
}

// �`��
void Title::Draw(void)
{
	func::DrawPmd(n);
}

// ����
void Title::UpData(void)
{
	func::Animation(n);
}
