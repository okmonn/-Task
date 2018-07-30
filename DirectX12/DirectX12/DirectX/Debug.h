#pragma once
#include "Obj.h"
#include <d3d12.h>

class Debug :
	public Obj
{
public:
	// コンストラクタ
	Debug();
	// デストラクタ
	~Debug();

private:
	// デバッグ
	ID3D12Debug * debug;
};

