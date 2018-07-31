#include "List.h"
#include "../Device.h"

// �R���X�g���N�^
List::List(std::weak_ptr<Device>dev) : allo(nullptr), list(nullptr)
{
	this->dev = dev;


	CreateAllo();
	CreateList();
}

// �f�X�g���N�^
List::~List()
{
	Release(list);
	Release(allo);
}

// �R�}���h�A���P�[�^�̐���
HRESULT List::CreateAllo(void)
{
	result = dev.lock()->Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allo));

	OutDebug(L"\n�R�}���h�A���P�[�^�̐����F���s\n", result);

	return result;
}

// �R�}���h���X�g�̐���
HRESULT List::CreateList(void)
{
	result = dev.lock()->Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, allo, nullptr, IID_PPV_ARGS(&list));

	OutDebug(L"\n�R�}���h���X�g�̐����F���s\n", result);

	list->Close();

	return result;
}
