#include "Depth.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Swap.h"
#include <tchar.h>

// �R���X�g���N�^
Depth::Depth(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap) : win(win)
{
	this->dev = dev;
	this->swap = swap;


	CreateResource();
	CreateView();
}

// �f�X�g���N�^
Depth::~Depth()
{
	Liberation();
}

// ���\�[�X����
HRESULT Depth::CreateResource(void)
{
	result = CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	if (FAILED(result))
	{
		return result;
	}

	//�v���p�e�B�ݒ�p�\���̂̐ݒ�
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\���̂̐ݒ�
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension          = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment          = 0;
	desc.Width              = win.lock()->GetX();
	desc.Height             = win.lock()->GetY();
	desc.DepthOrArraySize   = 1;
	desc.MipLevels          = 0;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags              = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//�N���A�l�ݒ�p�\���̂̐ݒ�
	D3D12_CLEAR_VALUE clear = {};
	clear.Format               = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	clear.DepthStencil.Depth   = 1.0f;
	clear.DepthStencil.Stencil = 0;

	resource.resize(sizeof(BYTE));

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, IID_PPV_ARGS(&resource[0]));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�[�x�X�e���V�����\�[�X�̐����F���s\n"));
	}

	return result;
}

// ���\�[�X�r���[�̐���
HRESULT Depth::CreateView(void)
{
	//�[�x�X�e���V���r���[�ݒ�p�\���̂̐ݒ�
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Format        = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
	desc.Flags         = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;

	//�[�x�X�e���V���r���[����
	dev.lock()->Get()->CreateDepthStencilView(resource[0], &desc, GetHeap()->GetCPUDescriptorHandleForHeapStart());

	return result;
}
