#pragma once
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Union;
class Device;
class List;

class Depth
{
public:
	// �R���X�g���N�^
	Depth(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// �f�X�g���N�^
	~Depth();

	// �[�x�X�e���V���̃Z�b�g
	void SetDepth(void);

	void SetShadow(void);

	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	ID3D12DescriptorHeap* GetSrvHeap(void) const {
		return srv;
	}
	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(void) const {
		return rsc;
	}
	ID3D12DescriptorHeap* GetShadowHeap(void) const {
		return shadowHeap;
	}
	ID3D12Resource* GetShadowRsc(void) const {
		return shadowRsc;
	}

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// SRV�q�[�v�̐���
	long CreateSrvHeap(void);

	// �V���h�E�q�[�v�̐���
	long CreateShadowHeap(void);

	// ���\�[�X�̐���
	long CreateRsc(void);

	// �V���h�E���\�[�X�̐���
	long CreateShadowRsc(void);

	// ���\�[�X�r���[�̐���
	void CreateView(void);

	// �V���h�E���\�[�X�r���[�̐���
	void CreateShadowView(void);

	// SRV�r���[�̐���
	void CreateSrvView(void);

	// �V���h�ESRV�̐���
	void CreateShadowSrv(void);

	// ������
	void Init(void);


	// ���j�I��
	Union& un;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// SRV�q�[�v
	ID3D12DescriptorHeap* srv;

	// ���\�[�X
	ID3D12Resource* rsc;

	// �V���h�E�q�[�v
	ID3D12DescriptorHeap* shadowHeap;

	// �V���h�E���\�[�X
	ID3D12Resource* shadowRsc;
};
