#include "List.h"
#include "../Device.h"
#include "Allocator.h"

// �R���X�g���N�^
List::List(std::weak_ptr<Device>dev, std::weak_ptr<Allocator>allo) : dev(dev), allo(allo), list(nullptr)
{
	Create();
}

// �f�X�g���N�^
List::~List()
{
	Release(list);
}

// �R�}���h���X�g�̐���
HRESULT List::Create(void)
{
	result = dev.lock()->Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, allo.lock()->Get(), nullptr, IID_PPV_ARGS(&list));

	OutDebug(L"\n�R�}���h���X�g�̐����F���s\n", result);

	return result;
}
