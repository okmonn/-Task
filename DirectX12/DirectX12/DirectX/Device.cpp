#include "Device.h"
#include <tchar.h>

// �@�\���x���ꗗ
D3D_FEATURE_LEVEL levels[] =
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
};

// �R���X�g���N�^
Device::Device() : dev(nullptr)
{
	Create();
}

// �f�X�g���N�^
Device::~Device()
{
	Release(dev);
}

// �f�o�C�X�̐���
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

	OutDebug(L"\n�f�o�C�X�̐����F���s\n", result);

	return result;
}
