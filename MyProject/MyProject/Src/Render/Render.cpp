#include "Render.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

//�N���A�J���[
const FLOAT color[] = {
	1.0f,
	0.0f,
	0.0f,
	0.0f
};

// �R���X�g���N�^
Render::Render(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap) :
	dev(dev), list(list), swap(swap), heap(nullptr), size(0)
{
	rsc.clear();

	CreateHeap();
	CreateRsc();
}

// �f�X�g���N�^
Render::~Render()
{
	for (auto itr = rsc.begin(); itr != rsc.end(); ++itr)
	{
		Release(*itr);
	}
	Release(heap);
}

// �q�[�v�̐���
long Render::CreateHeap(void)
{
	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = swap.lock()->GetCnt();
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�����_�[�^�[�Q�b�g�p�q�[�v�̐����F���s\n"));
		return hr;
	}

	//�q�[�v�T�C�Y�̊i�[
	size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(desc.Type);

	return hr;
}

// ���\�[�X�̐���
long Render::CreateRsc(void)
{
	//�z��̃������m��
	rsc.resize(swap.lock()->GetCnt());

	//�����_�[�^�[�Q�b�g�ݒ�p�\����
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	desc.Format               = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension        = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice   = 0;
	desc.Texture2D.PlaneSlice = 0;

	//�擪�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();

	auto hr = S_OK;

	for (UINT i = 0; i < rsc.size(); ++i)
	{
		hr = swap.lock()->Get()->GetBuffer(i, IID_PPV_ARGS(&rsc[i]));
		if (FAILED(hr))
		{
			OutputDebugString(_T("\n�o�b�t�@�̎擾�F���s\n"));
			break;
		}

		//�����_�[�^�[�Q�b�g�̐���
		dev.lock()->Get()->CreateRenderTargetView(rsc[i], &desc, handle);

		//�n���h���ʒu�̍X�V
		handle.ptr += size;
	}

	auto d = rsc[0]->GetDesc();
	{
		{
			firstHeap = nullptr;
			//�q�[�v�ݒ�p�\����
			D3D12_DESCRIPTOR_HEAP_DESC ddesc = {};
			ddesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ddesc.NodeMask = 0;
			ddesc.NumDescriptors = 2;
			ddesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

			auto hr = dev.lock()->Get()->CreateDescriptorHeap(&ddesc, IID_PPV_ARGS(&firstHeapRTV));

			auto hr = dev.lock()->Get()->CreateDescriptorHeap(&ddesc, IID_PPV_ARGS(&firstHeapRTV));
		}
		//�v���p�e�B�ݒ�p�\����
		D3D12_HEAP_PROPERTIES prop = {};
		prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
		prop.CreationNodeMask = 1;
		prop.VisibleNodeMask = 1;

		//���\�[�X�ݒ�p�\����
		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Width = d.Width;
		desc.Height = d.Height;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
		desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

		auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&first));
		
		auto handle = firstHeap->GetCPUDescriptorHandleForHeapStart();
		
		dev.lock()->Get()->CreateRenderTargetView(first, nullptr, handle);
		{
			handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			D3D12_SHADER_RESOURCE_VIEW_DESC ddesc = {};
			ddesc.Format = desc.Format;
			ddesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
			ddesc.Texture2D.MipLevels = 1;
			ddesc.Texture2D.MostDetailedMip = 0;
			ddesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			dev.lock()->Get()->CreateShaderResourceView(first, &ddesc, handle);
		}
		

	}

	return hr;
}

// �����_�[�^�[�Q�b�g�̃Z�b�g
void Render::SetRender(ID3D12DescriptorHeap & depth)
{
	//�q�[�v�̐擪�n���h���̎擾
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += size * swap.lock()->Get()->GetCurrentBackBufferIndex();

	//�����_�[�^�[�Q�b�g�̃Z�b�g
	list.lock()->GetList()->OMSetRenderTargets(1, &handle, false, &depth.GetCPUDescriptorHandleForHeapStart());

	//�����_�[�^�[�Q�b�g�̃N���A
	list.lock()->GetList()->ClearRenderTargetView(handle, color, 0, nullptr);
}
