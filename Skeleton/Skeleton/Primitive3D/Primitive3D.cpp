#include "Primitive3D.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Camera/Camera.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// コンストラクタ
Primitive3D::Primitive3D() : descMane(DescriptorMane::Get()),
constant(0), vRsc(0)
{
	vertex.clear();
}

// デストラクタ
Primitive3D::~Primitive3D()
{
}

// 定数リソースの生成
long Primitive3D::CreateRsc(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = (sizeof(WVP) + 0xff) &~0xff;

	return descMane.CreateRsc(dev, constant, prop, desc);
}

// 定数ビューの生成
void Primitive3D::CreateView(void)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(constant)->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(WVP) + 0xff) &~0xff;

	dev.lock()->Get()->CreateConstantBufferView(&desc, descMane.GetHeap(constant)->GetCPUDescriptorHandleForHeapStart());
}

// 定数のマップ
long Primitive3D::Map(void)
{
	void* data = nullptr;
	auto hr = descMane.GetRsc(constant)->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nプリミティブ用定数リソースのマップ：失敗\n"));
		return hr;
	}

	WVP wvp;
	DirectX::XMStoreFloat4x4(&wvp.world, DirectX::XMMatrixIdentity());
	wvp.view       = cam.lock()->GetView();
	wvp.projection = cam.lock()->GetProjection();
	wvp.lightView = cam.lock()->GetLightView();
	wvp.lightProjection = cam.lock()->GetLightProjection();

	memcpy(data, &wvp, sizeof(WVP));

	descMane.GetRsc(constant)->Unmap(0, nullptr);

	return hr;
}

// 頂点リソースの生成
long Primitive3D::CreateVertex(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1,0 };
	desc.Width            = sizeof(prm3d::Vertex) * vertex.size();

	return descMane.CreateRsc(dev, vRsc, prop, desc);
}
