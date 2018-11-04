#pragma once
#include "../etc/Vector2.h"

class Object
{
public:
	// �R���X�g���N�^
	Object();
	// �f�X�g���N�^
	virtual ~Object();

	// �`��
	virtual void Draw(void) = 0;

	// ���W�̎擾
	Vec2f GetPos(void) const {
		return pos;
	}
	// ���W�̃Z�b�g
	void SetPos(const Vec2f& pos) {
		this->pos = pos;
	}

	// �T�C�Y�̎擾
	Vec2f GetSize(void) const {
		return size;
	}
	// �T�C�Y�̃Z�b�g
	void SetSize(const Vec2f& size) {
		this->size = size;
	}

protected:
	// �摜�f�[�^
	int image;

	// ���W
	Vec2f pos;

	// �T�C�Y
	Vec2f size;
};
