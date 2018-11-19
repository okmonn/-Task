#include "Plane.h"
#include "../etc/Release.h"

// コンストラクタ
Plane::Plane(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe)
{
	this->dev = dev;
	this->list = list;
	this->con = con;
	this->root = root;
	this->pipe = pipe;

	SetVertex({ -10.0f, -0.2f, 10.0f }, {20.0f, 0.0f, -20.0f});
	CreateRsc();
	Map();
}

// デストラクタ
Plane::~Plane()
{
	Release(rsc);
}
