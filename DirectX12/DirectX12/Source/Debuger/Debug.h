#pragma once
#include "../BaseObj/Obj.h"

class Debug :
	public Obj
{
public:
	// コンストラクタ
	Debug();
	// デストラクタ
	~Debug();

private:
	// 生成
	HRESULT Create(void);


	// デバッグ
	ID3D12Debug* debug;
};

