#include "MultiRender.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

//�N���A�J���[
const FLOAT cler[] = {
	0.0f,
	0.0f,
	0.0f,
	0.0f
};


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
long MultiRender::CreateHeap(ID3D12DescriptorHeap ** heap, const D3D12_DESCRIPTOR_HEAP_TYPE & type)
{
	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
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
	auto size = render.lock()->GetRsc(0)->GetDesc();

	//�v���p�e�B�ݒ�p�\����
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.VisibleNodeMask      = 0;

	////���\�[�X�ݒ�p�\����
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width            = size.Width;
	desc.Height           = size.Height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, nullptr, IID_PPV_ARGS(&rsc));
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
	//list.lock()->GetList()->ClearRenderTargetView(rtv->GetCPUDescriptorHandleForHeapStart(), color, 0, nullptr);
}
