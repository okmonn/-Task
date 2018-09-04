#pragma once
#include "../Obj.h"
#include <memory>

class Device;
class Swap;
class Root;
class Compiler;

class PointPipe :
	public Obj
{
public:
	// コンストラクタ
	PointPipe();
	// デストラクタ
	~PointPipe();

private:
	// デバイス
	std::weak_ptr<Device>dev;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// ルートシグネチャ
	std::weak_ptr<Root>root;
};

