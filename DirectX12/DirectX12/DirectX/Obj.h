#pragma once
#include <d3d12.h>

// リリースマクロ
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

class Obj
{
public:
	// コンストラクタ
	Obj();
	// デストラクタ
	virtual ~Obj();

protected:
	// 参照結果
	HRESULT result;
};

