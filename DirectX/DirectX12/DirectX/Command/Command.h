#pragma once
#include "../Obj.h"
#include <memory>

class Device;

class Command : 
	public Obj
{
public:
	// コンストラクタ
	Command() {
	};
	// デストラクタ
	virtual ~Command() {
	};

protected:
	// デバイス
	std::weak_ptr<Device>dev;
};

