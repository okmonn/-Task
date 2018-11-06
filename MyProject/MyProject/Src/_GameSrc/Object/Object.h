#pragma once
#include "../etc/Vector2.h"
#include <memory>

class Camera;

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

	// ���S���W�̎擾
	Vec2f GetCenter(void) const {
		return { pos.x + size.x / 2.0f, pos.y + size.y / 2.0f };
	}

protected:
	// �J����
	std::weak_ptr<Camera>cam;

	// �摜�f�[�^
	int image;

	// ���W
	Vec2f pos;

	// �T�C�Y
	Vec2f size;

	// �؂�����W
	Vec2f rectPos;

	// �؂���T�C�Y
	Vec2f rectSize;
};
