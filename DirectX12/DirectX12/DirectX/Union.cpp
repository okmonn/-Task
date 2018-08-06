#include "Union.h"
#include "../Window/Window.h"
#ifdef _DEBUG
#include "Debug.h"
#endif
#include "Device.h"
#include "Command/Queue.h"
#include "Command/List.h"
#include "Swap.h"
#include "Descriptor/Render.h"
#include "Descriptor/Depth.h"
#include "Descriptor/Constant.h"
#include "RootSignature.h"
#include "Pipe.h"
#include "Fence.h"
#include "Barrier.h"
#include "Texture\Texture.h"
#include <tchar.h>

#pragma comment (lib, "d3d12.lib")

UINT m = 0;
UINT n = 0;

// �R���X�g���N�^
Union::Union() : x(x), y(y)
{
	msg = {};
	viewPort = {};
	scissor = {};
}

// �f�X�g���N�^
Union::~Union()
{
}

// �N���X�̃C���X�^���X��
void Union::Create(void)
{
	win      = std::make_shared<Window>(x, y);
#ifdef _DEBUG
	debug    = std::make_shared<Debug>();
#endif
	dev      = std::make_shared<Device>();
	queue    = std::make_shared<Queue>(dev);
	list     = std::make_shared<List>(dev);
	swap     = std::make_shared<Swap>(win, queue);
	render   = std::make_shared<Render>(dev, list, swap);
	depth    = std::make_shared<Depth>(win, dev, list, swap);
	constant = std::make_shared<Constant>(win, dev, list, swap);
	root     = std::make_shared<RootSignature>(dev/*, _T("Shader2D.hlsl"), "BasicVS", "BasicPS"*/);
	root->ComVer(_T("Shader2D.hlsl"), "BasicVS");
	root->ComPix(_T("Shader2D.hlsl"), "BasicPS");
	pipe     = std::make_shared<Pipe>(dev, swap, root);
	fence    = std::make_shared<Fence>(dev, queue);
	barrier  = std::make_shared<Barrier>(list, swap, render);
	tex = std::make_shared<Texture>(win, dev, list, swap);
	tex->LoadWIC(m, "img/sample.png");
	tex->LoadWIC(n, "img/sample.png");

	viewPort = { 0, 0, (FLOAT)this->win->GetX(), (FLOAT)this->win->GetY(), 0, 1 };
	scissor  = { 0, 0, static_cast<LONG>(this->win->GetX()), static_cast<LONG>(this->win->GetY()) };
}

// �E�B���h�E�̃T�C�Y�Z�b�g
void Union::ChangeWindowSize(UINT x, UINT y)
{
	this->x = x;
	this->y = y;
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
	//WVP�X�V
	constant->UpDataWVP();

	//���Z�b�g
	{
		list->GetAllo()->Reset();
		list->GetList()->Reset(list->GetAllo(), pipe->Get());
	}

	//���[�g�V�O�l�`���̃Z�b�g
	list->GetList()->SetGraphicsRootSignature(root->Get());

	//�p�C�v���C���̃Z�b�g
	list->GetList()->SetPipelineState(pipe->Get());

	//�萔�o�b�t�@�̃Z�b�g
	constant->SetConstant();

	//�r���[�̃Z�b�g
	list->GetList()->RSSetViewports(1, &viewPort);

	//�V�U�[�̃Z�b�g
	list->GetList()->RSSetScissorRects(1, &scissor);

	//�o���A�̐ݒu
	barrier->SetBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//�����_�[�^�[�Q�b�g�̃Z�b�g
	render->SetRender(depth->GetHeap());

	//�[�x�X�e���V���̃Z�b�g
	depth->SetDepth();

	tex->Draw(m/*, Vec2f(0,0), Vec2f(100,100)*/);

	tex->Draw(n
);
}

// ���s
void Union::Do(void)
{
	//�o���A�̐ݒu
	barrier->SetBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//�R�}���h���X�g�̋L�^�I��
	list->GetList()->Close();

	//�R�}���h���X�g�̎��s
	{
		//���X�g�̔z��
		ID3D12CommandList *commandList[] = { list->GetList() };
		//�z��łȂ��ꍇ�Fqueue->ExecuteCommandLists(1, (ID3D12CommandList*const*)&list);
		queue->Get()->ExecuteCommandLists(_countof(commandList), commandList);
	}

	//���A�\��ʂ𔽓]
	swap->Get()->Present(1, 0);

	//�ҋ@
	fence->Wait();
}
