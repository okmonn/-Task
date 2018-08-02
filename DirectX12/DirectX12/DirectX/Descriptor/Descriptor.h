#pragma once
#include "../Obj.h"
#include <vector>
#include <memory>

class Device;
class List;
class Swap;

class Descriptor : 
	public Obj
{
public:
	// �R���X�g���N�^
	Descriptor();
	// �f�X�g���N�^
	virtual ~Descriptor();

	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(UINT i = 0) const {
		return heap[i];
	}
	// �q�[�v�̎擾
	std::vector<ID3D12DescriptorHeap*> GetVecHeap(void) const {
		return heap;
	}

	// ���\�[�X�̎擾
	ID3D12Resource* GetResource(UINT i = 0) const {
		return resource[i];
	}
	// ���\�[�X�̎擾
	std::vector<ID3D12Resource*> GetVecResource(void) const {
		return resource;
	}

	// �q�[�v�T�C�Y�̎擾
	UINT GetSIze(void) const {
		return size;
	}

	// ���\�[�X�̉��
	void ResourceLiberation(UINT i, bool unmap = false);
	// �q�[�v�̉��
	void HeapLiberation(UINT i);

protected:
	// �q�[�v�̐���
	HRESULT CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	// ���Z�b�g
	void Reset(void);

	// ���
	void Liberation(bool unmap = false);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �X���b�v�`�F�C��
	std::weak_ptr<Swap>swap;

	// �q�[�v
	std::vector<ID3D12DescriptorHeap*>heap;

	// ���\�[�X
	std::vector<ID3D12Resource*>resource;

	// �q�[�v�T�C�Y
	UINT size;
};

