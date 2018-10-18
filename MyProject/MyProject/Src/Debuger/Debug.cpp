#include "Debug.h"
#include "../etc/Release.h"

// コンストラクタ
Debug::Debug() : 
	debug(nullptr)
{
	CreateDebug();
}

// デストラクタ
Debug::~Debug()
{
	Release(debug);
}

// デバッガの生成
long Debug::CreateDebug(void)
{
	auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nデバッガの生成：失敗\n"));
		return hr;
	}

	debug->EnableDebugLayer();

	return hr;
}
