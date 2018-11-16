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

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// SRV�q�[�v�̐���
	long CreateSrvHeap(void);

	// ���\�[�X�̐���
	long CreateRsc(void);

	// ���\�[�X�r���[�̐���
	void CreateView(void);

	// SRV�r���[�̐���
	void CreateSrvView(void);

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
};
