#include "Depth.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Swap.h"

// コンストラクタ
Depth::Depth(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap) : win(win)
{
	this->dev = dev;
	this->swap = swap;


	CreateResource();
	CreateView();
}

// デストラクタ
Depth::~Depth()
{
	Liberation();
}

// リソース生成
HRESULT Depth::CreateResource(void)
{
	result = CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	if (FAILED(result))
	{
		return result;
	}

	//プロパティ設定用構造体の設定
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体の設定
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

	//クリア値設定用構造体の設定
	D3D12_CLEAR_VALUE clear = {};
	clear.Format               = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	clear.DepthStencil.Depth   = 1.0f;
	clear.DepthStencil.Stencil = 0;

	resource.resize(sizeof(BYTE));

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, IID_PPV_ARGS(&resource[0]));

	OutDebug(L"\n深度ステンシルリソースの生成：失敗\n", result);

	return result;
}

// リソースビューの生成
HRESULT Depth::CreateView(void)
{
	//深度ステンシルビュー設定用構造体の設定
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Format        = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
	desc.Flags         = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;

	//深度ステンシルビュー生成
	dev.lock()->Get()->CreateDepthStencilView(resource[0], &desc, GetHeap()->GetCPUDescriptorHandleForHeapStart());

	return result;
}
