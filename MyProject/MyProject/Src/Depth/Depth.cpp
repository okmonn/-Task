#include "Depth.h"
#include "../Union/Union.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../etc/Release.h"

// リソース最大数
#define RSC_MAX 1

// コンストラクタ
Depth::Depth(std::weak_ptr<Device>dev, std::weak_ptr<List>list) :
	un(Union::Get()), dev(dev), list(list), heap(nullptr), rsc(nullptr)
{
	Init();
}

// デストラクタ
Depth::~Depth()
{
	Release(rsc);
	Release(heap);
}

// ヒープの生成
long Depth::CreateHeap(void)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = RSC_MAX;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n深度ステンシル用ヒープの生成：失敗\n"));
	}

	return hr;
}

// リソースの生成
long Depth::CreateRsc(void)
{
	//プロパティ設定用構造体の設定
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 0;

	//リソース設定用構造体の設定
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

	//クリア値設定用構造体の設定
	D3D12_CLEAR_VALUE clear = {};
	clear.DepthStencil.Depth   = 1.0f;
	clear.DepthStencil.Stencil = 0;
	clear.Format               = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n深度ステンシル用リソースの生成：失敗\n"));
	}

	return hr;
}

// リソースビューの生成
void Depth::CreateView(void)
{
	//深度ステンシルビュー設定用構造体の設定
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Flags              = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.Texture2D.MipSlice = 0;
	desc.ViewDimension      = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

	//深度ステンシルビュー生成
	dev.lock()->Get()->CreateDepthStencilView(rsc, &desc, heap->GetCPUDescriptorHandleForHeapStart());
}

// 初期化
void Depth::Init(void)
{
	CreateHeap();
	CreateRsc();
	CreateView();
}

// 深度ステンシルのセット
void Depth::SetDepth(void)
{
	list.lock()->GetList()->ClearDepthStencilView(heap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
