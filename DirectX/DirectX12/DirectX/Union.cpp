#include "Union.h"
#include "../Window/Window.h"
#include "Input/Input.h"
#ifdef _DEBUG
#include "Debug.h"
#endif
#include "Device.h"
#include "Command/Queue.h"
#include "Command/List.h"
#include "Swap.h"
#include "Descriptor/Render.h"
#include "Descriptor/Depth.h"
#include "Fence.h"
#include "Root.h"
#include "Pipe.h"
#include "Draw/Triangle.h"
#include "Descriptor/Constant.h"
#include "Texture/Texture.h"
#include <tchar.h>
#include "../Sound/WAVE/WAVE.h"

#pragma comment(lib, "d3d12.lib")

// �R���X�g���N�^
Union::Union()
{
	msg = {};
	viewPort = {};
	scissor = {};
	barrier = {};
}

// �f�X�g���N�^
Union::~Union()
{
}

// �E�B���h�E�̃T�C�Y�Z�b�g
void Union::ChangeWindowSize(UINT x, UINT y)
{
	this->x = x;
	this->y = y;
}

UINT n = 0;
UINT m = 0;
WAVE wave;

// �N���X�̃C���X�^���X��
void Union::Create(void)
{
	win      = std::make_shared<Window>(x, y);
	input    = std::make_shared<Input>(win);
#ifdef _DEBUG
	debug    = std::make_shared<Debug>();
#endif
	dev      = std::make_shared<Device>();
	queue    = std::make_shared<Queue>(dev);
	list     = std::make_shared<List>(dev);
	swap     = std::make_shared<Swap>(win, queue);
	render   = std::make_shared<Render>(dev, list, swap);
	depth    = std::make_shared<Depth>(win, dev, list, swap);
	fence    = std::make_shared<Fence>(dev, queue);
	root     = std::make_shared<Root>(dev);
	root->ComVertex(L"BasicShader.hlsl", "VS");
	root->ComPixel(L"BasicShader.hlsl", "PS");
	pipe     = std::make_shared<Pipe>(dev, swap, root);
	tri      = std::make_shared<Triangle>(dev, list);
	constant = std::make_shared <Constant>(win, dev, list);
	tex      = std::make_shared<Texture>(dev, list);

	tex->LoadWIC(n, "img/sample2.png");
	sound::LoadWave("Wave/sample.wav", wave);
	ViewPort();
	Scissor();
}

// �r���[�|�[�g�̃Z�b�g
void Union::ViewPort(void)
{
	viewPort.Height   = static_cast<FLOAT>(y);
	viewPort.MaxDepth = 1.0f;
	viewPort.MinDepth = 0.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width    = static_cast<FLOAT>(x);
}

// �V�U�[�̃Z�b�g
void Union::Scissor(void)
{
	scissor.bottom = static_cast<LONG>(y);
	scissor.left   = 0;
	scissor.right  = static_cast<LONG>(x);
	scissor.top    = 0;
}

// �o���A�̃Z�b�g
void Union::Barrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter)
{
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = render->GetResource(swap->Get()->GetCurrentBackBufferIndex());
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	//�o���A�ݒu
	list->GetList()->ResourceBarrier(1, &barrier);
}

// ���b�Z�[�W�̊m�F
bool Union::CheckMsg(void)
{
	if (msg.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		//�Ăяo�����X���b�h�����L���Ă���E�B���h�E�ɑ��M���ꂽ���b�Z�[�W�ۗ̕�����Ă��镨���擾
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
			TranslateMessage(&msg);
			//1�̃E�B�h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o����
			DispatchMessage(&msg);
		}
	}

	return true;
}

// �`�揀��
void Union::Set(void)
{
	constant->UpDataWVP();

	list->Reset(pipe->Get());

	list->SetRoot(root->Get());

	list->SetPipe(pipe->Get());

	constant->SetConstant();

	list->SetViewPort(viewPort);

	list->SetScissor(scissor);

	Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);

	render->SetRender(depth->GetHeap());

	depth->SetDepth();

	constant->UpDataImage();
	constant->SetConstant(1, 1);
	
	tex->Draw(n, { 100, 100 }, { 100, 100 }, { 0,0 }, { 512, 512 });
}

// ���s
void Union::Do(void)
{
	Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

	list->Close();

	//�R�}���h���s.
	ID3D12CommandList* ppCmdLists[] = { list->GetList() };
	queue->Get()->ExecuteCommandLists(_countof(ppCmdLists), ppCmdLists);

	swap->Present();

	fence->Wait();
}
