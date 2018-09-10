#include "Point.h"
#include "../../Window/Window.h"
#include "../../Device/Device.h"
#include "../../Command/List/List.h"
#include "../../RootSignature/Root.h"
#include "../../PipeLine/Pipe.h"
#include "../../Descriptor/ConstantBuffer/Constant.h"
#include <tchar.h>

//コンストラクタ
Point::Point(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<Constant>con) :
	win(win), root(root), pipe(pipe), con(con), resource(nullptr), data(nullptr)
{
	this->dev = dev;
	this->list = list;

	vertex.clear();
	view = {};
	vertexMax = this->win.lock()->GetX() * this->win.lock()->GetY();

	CreateResource();
}

// デストラクタ
Point::~Point()
{
	if (resource != nullptr)
	{
		resource->Unmap(0, nullptr);
	}
	Release(resource);
	Release(heap);
}

// リソースの生成
HRESULT Point::CreateResource(void)
{
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
	desc.Width            = sizeof(PointVertex) * vertexMax;
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
		OutputDebugString(_T("\nポイント用リソースの生成：失敗\n"));
		return result;
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	result = resource->Map(0, &range, reinterpret_cast<void**>(&data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nポイント用リソースのマッピング：失敗\n"));
		return result;
	}

	//頂点データのコピー
	memcpy(data, vertex.data(), sizeof(PointVertex) * vertex.size());

	//頂点バッファ設定用構造体の設定
	view.BufferLocation = resource->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(PointVertex) * vertex.size();
	view.StrideInBytes  = sizeof(PointVertex);

	return result;
}

// 頂点データの追加
void Point::AddList(const Vec2f& pos, const Vec3f& color, float alpha)
{
	vertex.push_back({ { pos.x, pos.y, 0.0f }, { color.x, color.y, color.z, alpha } });
}

// 描画
void Point::Draw(void)
{
	if (vertex.size() <= 0)
	{
		return;
	}

	list.lock()->SetRoot(root.lock()->Get());
	list.lock()->SetPipe(pipe.lock()->Get());

	con.lock()->SetConstant();

	//頂点データの更新
	memcpy(data, vertex.data(), sizeof(PointVertex) * vertex.size());

	//頂点バッファ設定用構造体の設定
	view.BufferLocation = resource->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(PointVertex) * vertex.size();
	view.StrideInBytes  = sizeof(PointVertex);

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//描画
	list.lock()->GetList()->DrawInstanced(vertex.size(), 1, 0, 0);
}

// 配列のリセット
void Point::Reset(void)
{
	vertex.clear();
	vertex.shrink_to_fit();
}
