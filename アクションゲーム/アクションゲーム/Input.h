#pragma once
class Input
{
public:
	// �R���X�g���N�^
	Input();
	// �f�X�g���N�^
	virtual ~Input();

	// ���݂̃L�[��Ԃ̃`�F�b�N
	bool CheckTrigger(int key)const;
	// ���݂̃L�[������Ԃ̃`�F�b�N
	bool CheckPress(int key)const;

	// ����
	void UpData(void);

private:
	// ���݂̃L�[���
	int state;

	// ���O�̃L�[���
	int old_state;
};

