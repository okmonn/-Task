#pragma once
#include "Primitive.h"

class Plane :
	public Primitive
{
public:
	// コンストラクタ
	Plane(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Plane();

private:

};
