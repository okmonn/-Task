#include "Input.h"
#include "DxLib.h"

// �R���X�g���N�^
Input::Input()
{
	//���݂̃L�[���
	state = 0;

	//���O�̃L�[���
	old_state = 0;
}

// �f�X�g���N�^
Input::~Input()
{
}

// ���݂̃L�[��Ԃ̃`�F�b�N
const bool Input::CheckTrigger(int key) const
{
	return (state & key) && !(old_state & key);
}

// ���݂̃L�[������Ԃ̃`�F�b�N
const bool Input::CheckPress(int key) const
{
	return state & key;
}

// ����
void Input::UpData(void)
{
	old_state = state;
	state = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}
