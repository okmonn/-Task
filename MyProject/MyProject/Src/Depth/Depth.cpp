#include "Depth.h"
#include "../Union/Union.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../etc/Release.h"

// ���\�[�X�ő吔
#define RSC_MAX 1

// �R���X�g���N�^
Depth::Depth(std::weak_ptr<Device>dev, std::weak_ptr<List>list) :
	un(Union::Get()), dev(dev), list(list), heap(nullptr), rsc(nullptr)
{
	Init();
}

// �f�X�g���N�^
Depth::~Depth()
{
	Release(rsc);
	Release(heap);
}

// �q�[�v�̐���
long Depth::CreateHeap(void)
{
	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = RSC_MAX;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�[�x�X�e���V���p�q�[�v�̐����F���s\n"));
	}

	return hr;
}

// ���\�[�X�̐���
long Depth::CreateRsc(void)
{
	//�v���p�e�B�ݒ�p�\���̂̐ݒ�
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 0;

	//���\�[�X�ݒ�p�\���̂̐ݒ�
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment          = 0;
	desc.DepthOrArraySize   = 1;
	desc.Dimension          = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags              = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	desc.Height             = un.GetWinY();
	desc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels          = 0;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width              = un.GetWinX();

	//�N���A�l�ݒ�p�\���̂̐ݒ�
	D3D12_CLEAR_VALUE clear = {};
	clear.DepthStencil.Depth   = 1.0f;
	clear.DepthStencil.Stencil = 0;
	clear.Format               = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�[�x�X�e���V���p���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// ���\�[�X�r���[�̐���
void Depth::CreateView(void)
{
	//�[�x�X�e���V���r���[�ݒ�p�\���̂̐ݒ�
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Flags              = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.Texture2D.MipSlice = 0;
	desc.ViewDimension      = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

	//�[�x�X�e���V���r���[����
	dev.lock()->Get()->CreateDepthStencilView(rsc, &desc, heap->GetCPUDescriptorHandleForHeapStart());
}

// ������
void Depth::Init(void)
{
	CreateHeap();
	CreateRsc();
	CreateView();
}

// �[�x�X�e���V���̃Z�b�g
void Depth::SetDepth(void)
{
	list.lock()->GetList()->ClearDepthStencilView(heap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}