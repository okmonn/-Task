#pragma once
#include "Object.h"
#include "../etc/Typedef.h"
#include <memory>

class Camera;


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

	// �X�e�[�^�X�̎擾
	State GetSt(void) const {
		return st;
	}
	// �X�e�[�^�X�̃Z�b�g
	void SetSt(const State& st) {
		this->st = st;
	}

protected:
	// �J����
	std::weak_ptr<Camera>cam;

	// �A�j���[�V�����ԍ�
	int index;

	// �t���[��
	int flam;

	// �ړ����x
	float speed;

	// ���[�J�����W
	Vec2f lpos;

	// �؂����`���W
	Vec2f rect;

	// �X�e�[�^�X
	State st;
};
