#include "MultiRender.h"
#include "../Union/Union.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../etc/Release.h"

// �R���X�g���N�^
MultiRender::MultiRender() : 
	rtv(nullptr), srv(nullptr), rsc(nullptr)
{
}

// �f�X�g���N�^
MultiRender::~MultiRender()
{
}

// �q�[�v�̐���
long MultiRender::CreateHeap(ID3D12DescriptorHeap ** heap, const D3D12_DESCRIPTOR_HEAP_TYPE & type, const D3D12_DESCRIPTOR_HEAP_FLAGS & flag)
{
	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = 1;
	desc.Type           = type;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&(*heap)));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�}���`�����_�[�̃q�[�v�̐����F���s\n"));
	}

	return hr;
}

// ���\�[�X�̐���
long MultiRender::CreateRsc(void)
{
	//�v���p�e�B�ݒ�p�\����
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\����
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width            = Union::Get().GetWinX();
	desc.Height           = Union::Get().GetWinY();
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//�N���A�l�ݒ�p�\����
	D3D12_CLEAR_VALUE clear = {};
	clear.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, &clear, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�}���`�����_�[�̃��\�[�X�̐����F���s\n"));
	}

	return hr;
}

// �����_�[�^�[�Q�b�g�̃Z�b�g
void MultiRender::SetRender(ID3D12DescriptorHeap & depth, const float * color)
{
	//�����_�[�^�[�Q�b�g�̃Z�b�g
	list.lock()->GetList()->OMSetRenderTargets(1, &rtv->GetCPUDescriptorHandleForHeapStart(), false, &depth.GetCPUDescriptorHandleForHeapStart());

	//�����_�[�^�[�Q�b�g�̃N���A
	list.lock()->GetList()->ClearRenderTargetView(rtv->GetCPUDescriptorHandleForHeapStart(), color, 0, nullptr);
}
