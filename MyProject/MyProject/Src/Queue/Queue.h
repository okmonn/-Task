#pragma once
#include <memory>

struct ID3D12CommandQueue;
class Device;

class Queue
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
	long Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�L���[
	ID3D12CommandQueue* queue;
};
