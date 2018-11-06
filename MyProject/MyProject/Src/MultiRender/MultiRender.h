#pragma once
#include <memory>
#include <DirectXMath.h>

enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_DESCRIPTOR_HEAP_FLAGS : int;

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;
class List;
class Render;
class Root;
class Pipe;

namespace multi {
	// ���_
	struct Vertex {
		//���W
		DirectX::XMFLOAT3 pos;
		//uv
		DirectX::XMFLOAT2 uv;
	};
}

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
	long CreateHeap(ID3D12DescriptorHeap** heap, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	// ���\�[�X�̐���
	long CreateRsc(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�胊�X�g
	std::weak_ptr<List>list;

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
