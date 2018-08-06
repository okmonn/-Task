#include "Triangle.h"
#include "../Device.h"
#include "../Command/List.h"
#include <tchar.h>

// 頂点の最大数
#define VERTEX_MAX 3

// コンストラクタ
Triangle::Triangle(std::weak_ptr<Device>dev, std::weak_ptr<List>list) :
	resource(nullptr), data(nullptr)
{
	this->dev = dev;
	this->list = list;

	vertex.clear();
	view = {};
	
	CreateResource();
}

// デストラクタ
Triangle::~Triangle()
{
	Release(resource);
}

// リソースの生成
HRESULT Triangle::CreateResource(void)
{
	//頂点の数分メモリを確保
	vertex.resize(VERTEX_MAX);
	vertex[0] = { { 200.0f, 200.0f, 0.0f }, { 0.0f, 0.0f } };//左上
	vertex[1] = { { 250.0f, 200.0f, 0.0f }, { 1.0f, 0.0f } };//右上
	vertex[2] = { { 250.0f, 250.0f, 0.0f }, { 1.0f, 1.0f } };//右下

	//プロパティ設定用構造体の設定
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体の設定
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width            = sizeof(Vertex) * vertex.size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n三角形の頂点リソースの生成：失敗\n"));
		return result;
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	result = resource->Map(0, &range, reinterpret_cast<void**>(&data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n三角形のリソースマッピング：失敗\n"));
		return result;
	}

	//頂点データのコピー
	memcpy(data, vertex.data(), sizeof(Vertex) * vertex.size());

	//アンマップ
	resource->Unmap(0, nullptr);

	//頂点バッファ設定用構造体の設定
	view.BufferLocation = resource->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(Vertex) * vertex.size();
	view.StrideInBytes  = sizeof(Vertex);

	return result;
}

// 描画
void Triangle::Draw(void)
{
	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//トポロジータイプのセット
	list.lock()->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画
	list.lock()->GetList()->DrawInstanced(VERTEX_MAX, 1, 0, 0);
}
