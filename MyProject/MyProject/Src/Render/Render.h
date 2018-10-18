#pragma once
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;
class List;
class Swap;

class Render
{
public:
	// �R���X�g���N�^
	Render(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Render();

	// �����_�[�^�[�Q�b�g�̃Z�b�g
	void SetRender(ID3D12DescriptorHeap& depth);

	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// ���\�[�X�̎擾
	std::vector<ID3D12Resource*> GetRsc(void) const {
		return rsc;
	}
	//���\�[�X�̎擾
	ID3D12Resource* GetRsc(const unsigned int& index) const {
		return rsc[index];
	}
	// �q�[�v�T�C�Y�̎擾
	unsigned int GetSize(void) const {
		return size;
	}

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// ���\�[�X�̐���
	long CreateRsc(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �X���b�v�`�F�C��
	std::weak_ptr<Swap>swap;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// ���\�[�X
	std::vector<ID3D12Resource*>rsc;

	// �q�[�v�T�C�Y
	unsigned int size;
};
