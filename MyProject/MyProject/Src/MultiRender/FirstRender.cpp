#include "FirstRender.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Depth/Depth.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// 頂点最大数
#define MAX 4  

// コンストラクタ
FirstRender::FirstRender(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Depth>dep, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) :
	vertex(nullptr), data(nullptr), dep(dep)
{
	this->dev = dev;
	this->list = list;
	this->root = root;
	this->pipe = pipe;

	v.clear();

	Init();
}

// デストラクタ
FirstRender::~FirstRender()
{
	Release(vertex);
	Release(rsc);
	Release(srv);
	Release(rtv);
}

// 初期化
void FirstRender::Init(void)
{
	CreateRsc();
	CreateRtv();
	CreateSrv();

	CreateVertex();
	Map();
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

// 頂点リソースの生成
long FirstRender::CreateVertex(void)
{
	v.resize(MAX);

	//プロパティ設定用構造体
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment          = 0;
	desc.DepthOrArraySize   = 1;
	desc.Dimension          = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags              = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height             = 1;
	desc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels          = 1;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width              = sizeof(multi::Vertex) * MAX;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertex));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nファーストパス用頂点リソースの生成：失敗\n"));
	}

	return hr;
}

// 頂点のマップ
long FirstRender::Map(void)
{
	auto hr = vertex->Map(0, nullptr, (void**)&data);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nファーストパス用頂点のマップ：失敗\n"));
	}

	return hr;
}

// 描画
void FirstRender::Draw(void)
{
	//コマンドリストへのセット
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	auto desc = rsc->GetDesc();

	//左上
	v[0] = { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } };
	//右上
	v[1] = { {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } };
	//左下
	v[2] = { { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } };
	//右下
	v[3] = { {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } };

	//頂点データの更新
	memcpy(data, v.data(), sizeof(multi::Vertex) * v.size());

	//頂点バッファ設定用構造体の設定
	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = vertex->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(multi::Vertex) * v.size();
	view.StrideInBytes  = sizeof(multi::Vertex);

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ボックス設定用構造体の設定
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = vertex->GetDesc().Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(vertex->GetDesc().Width);
	box.top = 0;

	//ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &srv);

	//ディスクラプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(0, srv->GetGPUDescriptorHandleForHeapStart());

	/*auto d = dep.lock()->GetSrvHeap();
	list.lock()->GetList()->SetDescriptorHeaps(1, &d);
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(1, d->GetGPUDescriptorHandleForHeapStart());*/

	//描画
	list.lock()->GetList()->DrawInstanced(v.size(), 1, 0, 0);
}
