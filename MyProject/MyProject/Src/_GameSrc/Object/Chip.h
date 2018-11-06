#pragma once
#include "Object.h"
#include <string>

class Chip :
	public Object
{
public:
	// �R���X�g���N�^
	Chip(const std::string& fileName, const int& id, std::weak_ptr<Camera>cam, const Vec2f& rectPos, const Vec2f& rectSize, const Vec2f& pos, const Vec2f& size);
	// �f�X�g���N�^
	~Chip();

	// �`��
	void Draw(void);

private:

};
