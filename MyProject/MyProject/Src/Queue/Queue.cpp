#include "Queue.h"
#include "../Device/Device.h"
#include "../etc/Release.h"

// �R���X�g���N�^
Queue::Queue(std::weak_ptr<Device>dev) : 
	dev(dev), queue(nullptr)
{
	Create();
}

// �f�X�g���N�^
Queue::~Queue()
{
	Release(queue);
}

// �R�}���h�L���[�̐���
long Queue::Create(void)
{
	//�R�}���h�L���[�ݒ�p�\����
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;

	auto hr = dev.lock()->Get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R�}���h�L���[�̐����F���s\n"));
	}

	return hr;
}

// �R�}���h�̎��s
void Queue::Execute(ID3D12CommandList ** list, const unsigned int & num)
{
	if (list == nullptr)
	{
		return;
	}

	queue->ExecuteCommandLists(num, list);
}