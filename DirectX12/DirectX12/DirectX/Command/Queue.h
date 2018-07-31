#pragma once
#include "Command.h"
#include <memory>

class Queue : 
	public Command
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


	// �R�}���h�L���[
	ID3D12CommandQueue* queue;
};

