#include "Allocator.h"
#include "../Device.h"

// �R���X�g���N�^
Allocator::Allocator(std::weak_ptr<Device>dev) : dev(dev), allo(nullptr)
{
}

// �f�X�g���N�^
Allocator::~Allocator()
{
	Release(allo);
}

// �R�}���h�A���P�[�^�̐���
HRESULT Allocator::Create(void)
{
	result = dev.lock()->Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allo));
	
	OutDebug(L"\n�R�}���h�A���P�[�^�̐����F���s\n", result);

	return result;
}
