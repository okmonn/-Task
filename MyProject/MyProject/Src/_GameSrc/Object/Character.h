#pragma once
#include "Object.h"
#include "../etc/Typedef.h"

class Character :
	public Object
{
public:
	// �R���X�g���N�^
	Character();
	// �f�X�g���N�^
	virtual ~Character();

	// �`��
	virtual void Draw(void) = 0;

	// �X�e�[�^�X�̃Z�b�g
	void SetState(const State& st);

	// �X�e�[�^�X�̎擾
	State GetSt(void) const {
		return st;
	}
	// �X�e�[�^�X�̃Z�b�g
	void SetSt(const State& st) {
		this->st = st;
	}

protected:
	// �A�j���[�V�����ԍ�
	int index;

	// �t���[��
	int flam;

	// �ړ����x
	float speed;

	// ���]�t���O
	bool reverse;

	// ���[�J�����W
	Vec2f lpos;

	// �O�̍��W
	Vec2f oldPos;

	// �X�e�[�^�X
	State st;
};
