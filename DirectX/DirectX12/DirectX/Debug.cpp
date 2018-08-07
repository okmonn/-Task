#include "Debug.h"
#include <tchar.h>

// コンストラクタ
Debug::Debug() : debug(nullptr)
{
	result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nデバッグの生成：失敗\n"));
	}

	//デバッグの表示を許可
	debug->EnableDebugLayer();
}

// デストラクタ
Debug::~Debug()
{
	Release(debug);
}
