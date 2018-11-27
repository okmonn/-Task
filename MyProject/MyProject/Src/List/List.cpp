#include "List.h"
#include "../Union/Union.h"
#include "../Device/Device.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../MultiRender/MultiRender.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// �R���X�g���N�^
List::List(std::weak_ptr<Device>dev) : 
	un(Union::Get()), dev(dev), allo(nullptr), list(nullptr)
{
	CreateAllo();
	CreateList();
}

// �f�X�g���N�^
List::~List()
{
	Release(list);
	Release(allo);
}

// �R�}���h�A���P�[�^�̐���
long List::CreateAllo(void)
{
	auto hr = dev.lock()->Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R�}���h�A���P�[�^�̐����F���s\n"));
	}

	return hr;
}

// �R�}���h���X�g�̐���
long List::CreateList(void)
{
	auto hr = dev.lock()->Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R�}���h���X�g�̐����F���s\n"));
		return hr;
	}

	Close();

	return hr;
}

// ���Z�b�g
void List::Reset(ID3D12PipelineState * pipe)
{
	allo->Reset();
	list->Reset(allo, pipe);
}

// ���[�g�V�O�l�`���̃Z�b�g
void List::SetRoot(ID3D12RootSignature & root)
{
	list->SetGraphicsRootSignature(&root);
}

// �p�C�v���C���̃Z�b�g
void List::SetPipe(ID3D12PipelineState & pipe)
{
	list->SetPipelineState(&pipe);
}

// �r���[�|�[�g�̃Z�b�g
void List::SetViewport(void)
{
	//�r���[�|�[�g�ݒ�p�\����
	D3D12_VIEWPORT view = {};
	view.Height   = static_cast<float>(un.GetWinY());
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width    = static_cast<float>(un.GetWinX());

	list->RSSetViewports(1, &view);
}

void List::SetViewport(const unsigned & w, const unsigned & h)
{
	D3D12_VIEWPORT view = {};
	view.Height = static_cast<float>(h);
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width = static_cast<float>(w);

	list->RSSetViewports(1, &view);
}

// �V�U�[�̃Z�b�g
void List::SetScissor(void)
{
	//�V�U�[�ݒ�p�\����
	RECT scissor = {};
	scissor.bottom = static_cast<LONG>(un.GetWinY());
	scissor.left   = 0;
	scissor.right  = static_cast<LONG>(un.GetWinX());
	scissor.top    = 0;

	list->RSSetScissorRects(1, &scissor);
}

void List::SetScissor(const unsigned & w, const unsigned & h)
{
	RECT scissor = {};
	scissor.bottom = static_cast<LONG>(h);
	scissor.left = 0;
	scissor.right = static_cast<LONG>(w);
	scissor.top = 0;

	list->RSSetScissorRects(1, &scissor);
}

// �o���A�̃Z�b�g
void List::SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, std::weak_ptr<Swap>swap, std::weak_ptr<Render>render)
{
	//�o���A�̐ݒ�
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = render.lock()->GetRsc(swap.lock()->Get()->GetCurrentBackBufferIndex());
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}

// �o���A�̃Z�b�g
void List::SetBarrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, std::weak_ptr<MultiRender> multi)
{
	//�o���A�̐ݒ�
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = multi.lock()->GetRsc();
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}

// �o���A�̃Z�b�g
void List::SetBarrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc)
{
	//�o���A�̐ݒ�
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = rsc;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}

// �R�}���h�����
void List::Close(void)
{
	list->Close();
}
