#pragma once
#include "../Obj.h"
#include <memory>

class Device;

class Command : 
	public Obj
{
public:
	Command() {
	};
	virtual ~Command() {
	};

protected:
	// �f�o�C�X
	std::weak_ptr<Device>dev;
};

