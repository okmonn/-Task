#pragma once
#include "Object.h"
#include <string>
#include <memory>

class Camera;

class Chip :
	public Object
{
public:
	// �R���X�g���N�^
	Chip(const std::string& fileName, const int& id, std::weak_ptr<Camera>cam, const Vec2f& rect = 0.0f, const Vec2f& pos = 0.0f, const Vec2f& size = 0.0f);
	// �f�X�g���N�^
	~Chip();

	// �`��
	void Draw(void);

	// �摜�̐؂����`�̃Z�b�g
	void SetRect(const float& x, const float& y) {
		rect = { x, y };
	}

private:
	// �J����
	std::weak_ptr<Camera>cam;

	// �؂����`
	Vec2f rect;
};
