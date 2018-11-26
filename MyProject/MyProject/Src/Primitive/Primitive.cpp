#include "Primitive.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Depth/Depth.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../etc/Release.h"

// コンストラクタ
Primitive::Primitive() : 
	rsc(nullptr), data(nullptr)
{
	vertex.clear();
}

// デストラクタ
Primitive::~Primitive()
{
}

// 頂点セット
void Primitive::SetVertex(const DirectX::XMFLOAT3 & pos, const DirectX::XMFLOAT3& size)
{
	vertex.resize(4);
	vertex[0] = { {pos.x,          pos.y,          pos.z },         {0,1,0} , {0,0} };
	vertex[1] = { {pos.x + size.x, pos.y,          pos.z },         {0,1,0} ,{1,0} };
	vertex[2] = { {pos.x,          pos.y + size.y, pos.z + size.z}, {0,1,0} , {0,1} };
	vertex[3] = { {pos.x + size.x, pos.y + size.y, pos.z + size.z}, {0,1,0} , {1,1} };
}

// リソースの生成
long Primitive::CreateRsc(void)
{
	//プロパティ設定用構造体
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask = 1;

	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width = sizeof(VERTEX) * 4;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nプリミティブ用頂点リソースの生成：失敗\n"));
	}

	return hr;
}

// マップ
long Primitive::Map(void)
{
	auto hr = rsc->Map(0, nullptr, &data);
	if (FAILED(hr))
	{
		return hr;
	}

	memcpy(data, &vertex[0], sizeof(VERTEX) * 4);

	rsc->Unmap(0, nullptr);

	return hr;
}

// 描画
void Primitive::Draw(void)
{
	//コマンドリストへのセット
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	list.lock()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		depth.lock()->GetRsc());

	con.lock()->SetConstant();

	auto h = depth.lock()->GetSrvHeap();
	list.lock()->GetList()->SetDescriptorHeaps(1, &h);
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(1, h->GetGPUDescriptorHandleForHeapStart());

	//頂点バッファ設定用構造体の設定
	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = rsc->GetGPUVirtualAddress();
	view.SizeInBytes = sizeof(VERTEX) * 4;
	view.StrideInBytes = sizeof(VERTEX);

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//描画
	list.lock()->GetList()->DrawInstanced(vertex.size(), 1, 0, 0);

	list.lock()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE,
		depth.lock()->GetRsc());
}
