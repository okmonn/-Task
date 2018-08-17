#include "Func.h"
#include "DirectX/Union.h"

// �E�B���h�E�T�C�Y�̃Z�b�g
void func::SetWindowSize(unsigned int x, unsigned int y)
{
	Union::Get().ChangeWindowSize(x, y);
}

// ������
void func::Init(void)
{
	Union::Get().Create();
}

// ���b�Z�[�W�̊m�F
bool func::CheckMsg(void)
{
	return Union::Get().CheckMsg();
}

// �`�揀��
void func::Set(void)
{
	Union::Get().Set();
}

// ���s
void func::Do(void)
{
	Union::Get().Do();
}

// �L�[����
bool func::CheckKey(unsigned int index)
{
	return Union::Get().CheckKey(index);
}

// �g���K�[�L�[����
bool func::TriggerKey(unsigned int index)
{
	return Union::Get().TriggerKey(index);
}

// �摜�ǂݍ���
void func::LoadImg(unsigned int & index, const std::string & fileName)
{
	Union::Get().LoadImg(index, fileName);
}

// �`��
void func::Draw(unsigned int & index, float x, float y, bool turnX, bool turnY)
{
	Union::Get().Draw(index, { x, y }, turnX, turnY);
}

// �`��E�T�C�Y�w��
void func::Draw(unsigned int & index, float x, float y, float sizeX, float sizeY, bool turnX, bool turnY)
{
	Union::Get().Draw(index, { x, y }, { sizeX, sizeY }, turnX, turnY);
}

// �`��E�T�C�Y�w��E����
void func::Draw(unsigned int & index, float x, float y, float sizeX, float sizeY, float rectX, float rectY, float rectSizeX, float rectSizeY, bool turnX, bool turnY)
{
	Union::Get().Draw(index, { x, y }, { sizeX, sizeY }, { rectX, rectY }, { rectSizeX, rectSizeY }, turnX, turnY);
}