#pragma once
#include <memory>

enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_DESCRIPTOR_HEAP_FLAGS : int;

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;
class Render;

class MultiRender
{
public:
	// �R���X�g���N�^
	MultiRender();
	// �f�X�g���N�^
	virtual ~MultiRender();

	// �����_�[�^�[�Q�b�g�p�q�[�v�̎擾
	ID3D12DescriptorHeap* GetRtv(void) const {
		return rtv;
	}
	// �V�F�[�_�[�p�q�[�v�̎擾
	ID3D12DescriptorHeap* GetSrv(void) const {
		return srv;
	}
	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(void) const {
		return rsc;
	}

protected:
	// �q�[�v�̐���
	long CreateHeap(ID3D12DescriptorHeap** heap, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	// ���\�[�X�̐���
	long CreateRsc(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �����_�[�^�[�Q�b�g
	std::weak_ptr<Render>render;

	// �����_�[�^�[�Q�b�g�p�q�[�v
	ID3D12DescriptorHeap* rtv;

	// �V�F�[�_�[�p�q�[�v
	ID3D12DescriptorHeap* srv;

	// ���\�[�X
	ID3D12Resource* rsc;
};
