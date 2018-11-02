#include "FirstRender.h"
#include "../Device/Device.h"
#include "../Render/Render.h"
#include "../etc/Release.h"

// コンストラクタ
FirstRender::FirstRender(std::weak_ptr<Device>dev, std::weak_ptr<Render>render)
{
	this->dev = dev;
	this->render = render;

	Init();
}

// デストラクタ
FirstRender::~FirstRender()
{
	Release(rsc);
	Release(srv);
	Release(rtv);
}

// 初期化
void FirstRender::Init(void)
{
	CreateRtv();
	CreateRsc();
	CreateSrv();
}

// レンダーターゲットビューの生成
void FirstRender::CreateRtv(void)
{
	if (FAILED(CreateHeap(&rtv, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE)))
	{
		return;
	}

	//レンダーターゲット設定用構造体
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	desc.Format               = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension        = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice   = 0;
	desc.Texture2D.PlaneSlice = 0;

	dev.lock()->Get()->CreateRenderTargetView(rsc, &desc, rtv->GetCPUDescriptorHandleForHeapStart());
}

// シェーダーリソースビューの生成
void FirstRender::CreateSrv(void)
{
	if (FAILED(CreateHeap(&srv, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)))
	{
		return;
	}

	//シェーダリソースビュー設定用構造体
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	dev.lock()->Get()->CreateShaderResourceView(rsc, &desc, srv->GetCPUDescriptorHandleForHeapStart());
}
