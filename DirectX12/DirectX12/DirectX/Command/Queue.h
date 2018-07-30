#pragma once
#include "../Obj.h"
#include <memory>

class Device;

class Queue : 
	public Obj
{
public:
	// �R���X�g���N�^
	Queue(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~Queue();

	// �R�}���h�L���[�̎擾
	ID3D12CommandQueue* Get(void) const {
		return queue;
	}

private:
	// �R�}���h�L���[�̐���
	HRESULT Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�L���[
	ID3D12CommandQueue* queue;
};

