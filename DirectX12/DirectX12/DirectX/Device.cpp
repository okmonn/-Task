#include "Device.h"
#include <tchar.h>

// 機能レベル一覧
D3D_FEATURE_LEVEL levels[] =
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
};

// コンストラクタ
Device::Device() : dev(nullptr)
{
	Create();
}

// デストラクタ
Device::~Device()
{
	Release(dev);
}

// デバイスの生成
HRESULT Device::Create(void)
{
	for (auto& i : levels)
	{
		result = D3D12CreateDevice(nullptr, i, IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			level = i;
			break;
		}
	}

	OutDebug(L"\nデバイスの生成：失敗\n", result);

	return result;
}
