#pragma once
#include "Primitive.h"

class Plane :
	public Primitive
{
public:
	// �R���X�g���N�^
	Plane(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Plane();

private:

};
