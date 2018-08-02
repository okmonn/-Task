#include "Barrier.h"
#include "Command\List.h"
#include "Swap.h"
#include "Descriptor\Render.h"

// �R���X�g���N�^
Barrier::Barrier(std::weak_ptr<List>list, std::weak_ptr<Swap>swap, std::weak_ptr<Render>render) : list(list), swap(swap), render(render)
{
	barrier = {};
}

// �f�X�g���N�^
Barrier::~Barrier()
{
}

// �o���A�̃Z�b�g
void Barrier::SetBarrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter)
{
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = render.lock()->GetVecResource()[swap.lock()->Get()->GetCurrentBackBufferIndex()];
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	//�o���A�ݒu
	list.lock()->GetList()->ResourceBarrier(1, &barrier);
}
