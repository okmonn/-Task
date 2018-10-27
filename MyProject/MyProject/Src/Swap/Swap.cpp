#include "Swap.h"
#include "../Union/Union.h"
#include "../Window/Window.h"
#include "../Queue/Queue.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

#pragma comment(lib, "dxgi.lib")

// �R���X�g���N�^
Swap::Swap(std::weak_ptr<Window>win, std::weak_ptr<Queue>queue) :
	un(Union::Get()), win(win), queue(queue), swap(nullptr), cnt(0)
{
	Create();
}

// �f�X�g���N�^
Swap::~Swap()
{
	Release(swap);
}

// �X���b�v�`�F�C���̐���
long Swap::Create(void)
{
	IDXGIFactory4* factory = nullptr;

	//�X���b�v�`�F�C���ݒ�p�\����
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = 2;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = 0;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = un.GetWinY();
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = un.GetWinX();

	//�t�@�N�g���[����
	auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�t�@�N�g���[�̐����F���s\n"));
	}

	//�o�b�N�o�b�t�@���̊i�[
	cnt = desc.BufferCount;

	//�X���b�v�`�F�C������
	hr = factory->CreateSwapChainForHwnd(queue.lock()->Get(), win.lock()->Get(), &desc, nullptr, nullptr, (IDXGISwapChain1**)(&swap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�X���b�v�`�F�C���̐����F���s\n"));
	}

	Release(factory);

	return 0;
}

// �\�E���̔��]
void Swap::Present(void)
{
	swap->Present(1, 0);
}
