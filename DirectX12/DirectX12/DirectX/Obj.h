#pragma once
#include "../Tstring.h"
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
	// デバッグ用出力
	void OutDebug(const std::tstring tex, HRESULT result);


	// 参照結果
	HRESULT result;
};

