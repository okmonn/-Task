#include "Obj.h"
#include <tchar.h>

// コンストラクタ
Obj::Obj() : result(S_OK)
{
}

// デストラクタ
Obj::~Obj()
{
}

// デバッグ用出力
void Obj::OutDebug(const std::tstring tex, HRESULT result)
{
#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugString(tex.c_str());
	}
#endif
}
