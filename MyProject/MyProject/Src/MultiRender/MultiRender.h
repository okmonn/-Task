#pragma once
#include <memory>

enum D3D12_DESCRIPTOR_HEAP_TYPE : int;

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;
class List;
class Swap;
class Render;
class Root;
class Pipe;

class MultiRender
{
public:
	// �R���X�g���N�^
	MultiRender();
	// �f�X�g���N�^
	virtual ~MultiRender();

	// �����_�[�^�[�Q�b�g�̃Z�b�g
	void SetRender(ID3D12DescriptorHeap& depth, const float* color);

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
	long CreateHeap(ID3D12DescriptorHeap** heap, const D3D12_DESCRIPTOR_HEAP_TYPE& type);

	// ���\�[�X�̐���
	long CreateRsc(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�胊�X�g
	std::weak_ptr<List>list;

	// �X���b�v�`�F�C��
	std::weak_ptr<Swap>swap;

	// �����_�[�^�[�Q�b�g
	std::weak_ptr<Render>render;

	// ���[�g�V�O�l�`��
	std::weak_ptr<Root>root;

	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// �����_�[�^�[�Q�b�g�p�q�[�v
	ID3D12DescriptorHeap* rtv;

	// �V�F�[�_�[�p�q�[�v
	ID3D12DescriptorHeap* srv;

	// ���\�[�X
	ID3D12Resource* rsc;
};
