#include "Debug.h"
#include "../etc/Release.h"

// �R���X�g���N�^
Debug::Debug() : 
	debug(nullptr)
{
	CreateDebug();
}

// �f�X�g���N�^
Debug::~Debug()
{
	Release(debug);
}

// �f�o�b�K�̐���
long Debug::CreateDebug(void)
{
	auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�f�o�b�K�̐����F���s\n"));
		return hr;
	}

	debug->EnableDebugLayer();

	return hr;
}
