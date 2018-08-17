#pragma once
#ifndef _NOT_USE_D3D12
#include <d3d12.h>
#endif

// リリースマクロ
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

class Obj
{
public:
	// コンストラクタ
	Obj() {
	};
	// デストラクタ
	virtual ~Obj() {
	};

protected:
	// 参照結果
	HRESULT result;
};
