#pragma once
#include "../Obj.h"
#include <memory>

class Device;

class Allocator :
	public Obj
{
public:
	// �R���X�g���N�^
	Allocator(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~Allocator();

	// �R�}���h�A���P�[�^�̎擾
	ID3D12CommandAllocator* Get(void) const {
		return allo;
	}

private:
	// �R�}���h�A���P�[�^�̐���
	HRESULT Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�A���P�[�^
	ID3D12CommandAllocator * allo;
};

