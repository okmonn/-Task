#include "MultiRender.h"
#include "../Device/Device.h"
#include "../Render/Render.h"
#include "../etc/Release.h"

// コンストラクタ
MultiRender::MultiRender() : 
	rtv(nullptr), srv(nullptr), rsc(nullptr)
{
}

// デストラクタ
MultiRender::~MultiRender()
{
}

// ヒープの生成
long MultiRender::CreateHeap(ID3D12DescriptorHeap ** heap, const D3D12_DESCRIPTOR_HEAP_TYPE & type, const D3D12_DESCRIPTOR_HEAP_FLAGS & flag)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = 1;
	desc.Type           = type;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&(*heap)));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nマルチレンダーのヒープの生成：失敗\n"));
	}

	return hr;
}

// リソースの生成
long MultiRender::CreateRsc(void)
{
	auto size = render.lock()->GetRsc(0)->GetDesc();

	//プロパティ設定用構造体
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width            = size.Width;
	desc.Height           = size.Height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nマルチレンダーのリソースの生成：失敗\n"));
	}

	return hr;
}
