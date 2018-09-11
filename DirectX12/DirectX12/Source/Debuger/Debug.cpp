#include "Debug.h"
#include <tchar.h>

// コンストラクタ
Debug::Debug() :
	debug(nullptr)
{
	Create();
}

// デストラクタ
Debug::~Debug()
{
	Release(debug);
}

// 生成
HRESULT Debug::Create(void)
{
	result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nデバッガの生成：失敗\n"));
		return result;
	}

	debug->EnableDebugLayer();

	return result;
}
