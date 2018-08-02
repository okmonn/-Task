#include "Descriptor.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include <tchar.h>

// �R���X�g���N�^
Descriptor::Descriptor() : size(0)
{
	Reset();
}

// �f�X�g���N�^
Descriptor::~Descriptor()
{
}

// �q�[�v�̐���
HRESULT Descriptor::CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag)
{
	//�_�~�[�錾
	ID3D12DescriptorHeap* dummy = nullptr;

	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = swap.lock()->GetBack();
	desc.Type           = type;

	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&dummy));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�q�[�v�̐����F���s\n"));
	}

	//�q�[�v�̒ǉ�
	heap.push_back(dummy);

	//�q�[�v�T�C�Y�̍X�V
	size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(type);

	//�_�~�[�̉��
	//Release(dummy);

	return result;
}

// ���Z�b�g
void Descriptor::Reset(void)
{
	heap.clear();
	resource.clear();
}

// ���\�[�X�̉��
void Descriptor::ResourceLiberation(UINT i, bool unmap)
{
	if (unmap == true)
	{
		resource[i]->Unmap(0, nullptr);
	}
	Release(resource[i]);
}

// �q�[�v�̉��
void Descriptor::HeapLiberation(UINT i)
{
	Release(heap[i]);
}

// ���
void Descriptor::Liberation(bool unmap)
{
	for (UINT i = 0; i < resource.size(); ++i)
	{
		if (unmap == true)
		{
			resource[i]->Unmap(0, nullptr);
		}
		Release(resource[i]);
	}

	for (UINT i = 0; i < heap.size(); ++i)
	{
		Release(heap[i]);
	}
}
