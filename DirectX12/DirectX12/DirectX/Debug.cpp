#include "Debug.h"
#include <tchar.h>

// コンストラクタ
Debug::Debug() : debug(nullptr)
{
	result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	OutDebug(L"\nデバッグの生成：失敗\n", result);

	debug->EnableDebugLayer();
}

// デストラクタ
Debug::~Debug()
{
	Release(debug);
}
