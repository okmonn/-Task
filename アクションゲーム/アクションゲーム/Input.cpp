#include "Input.h"
#include "DxLib.h"

// �R���X�g���N�^
Input::Input()
{
	state = 0;
	old_state = 0;
}

// �f�X�g���N�^
Input::~Input()
{
}

// ���݂̃L�[��Ԃ̃`�F�b�N
bool Input::CheckTrigger(int key) const
{

	return false;
}

// ���݂̃L�[������Ԃ̃`�F�b�N
bool Input::CheckPress(int key) const
{

	return false;
}

// ����
void Input::UpData(void)
{

}
