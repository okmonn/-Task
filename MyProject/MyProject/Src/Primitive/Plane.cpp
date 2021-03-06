#include "Plane.h"
#include "../etc/Release.h"

// コンストラクタ
Plane::Plane(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Depth>dep, std::weak_ptr<Constant>con, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe)
{
	this->dev = dev;
	this->list = list;
	this->con = con;
	this->depth = dep;
	this->root = root;
	this->pipe = pipe;

	SetVertex({ -25.0f, -1.0f, 25.0f }, {50.0f, 1.0f, -50.0f});
	CreateRsc();
	Map();
}

// デストラクタ
Plane::~Plane()
{
	Release(rsc);
}
