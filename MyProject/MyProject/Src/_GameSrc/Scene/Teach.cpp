#include "Teach.h"
#include "../../Func/Func.h"

// �R���X�g���N�^
Teach::Teach() : 
	model(0), x(0.0f), y(10.0f), z(-15.0f)
{
	func::LoadPmd("Model/�����~�N.pmd", model);
	func::Attach("���S�R���_���X.vmd", model);
}

// �f�X�g���N�^
Teach::~Teach()
{
}

// �`��
void Teach::Draw(void)
{
	func::DrawPmd(model);
}

// ����
void Teach::UpData(void)
{
	func::ChangeWVP(x, y, z, 0, 10, 0);
	if (func::CheckKey(INPUT_RIGHT))
	{
		++x;
	}
	else if (func::CheckKey(INPUT_LEFT))
	{
		--x;
	}
	else if (func::CheckKey(INPUT_DOWN))
	{
		--y;
	}
	else if (func::CheckKey(INPUT_UP))
	{
		++y;
	}
	else if (func::CheckKey(INPUT_W))
	{
		++z;
	}
	else if (func::CheckKey(INPUT_S))
	{
		--z;
	}
	func::Animation(model, true);
}
