#include "Texture.h"
#include "WICTextureLoader12.h"
#include "../Device.h"
#include "../Command/List.h"
#include <tchar.h>

// 頂点の最大数
#define VERTEX_MAX 4

// コンストラクタ
Texture::Texture(std::weak_ptr<Device>dev, std::weak_ptr<List>list)
{
	this->dev = dev;
	this->list = list;

	wic.clear();

	//WICの初期処理
	CoInitialize(nullptr);
}

// デストラクタ
Texture::~Texture()
{
	for (auto itr = wic.begin(); itr != wic.end(); ++itr)
	{
		itr->second.resource->Unmap(0, nullptr);
		itr->second.decode.release();
		Release(itr->second.resource);
		Release(itr->second.con.resource);
		Release(itr->second.con.heap);
	}
}

// WIC読み込み
HRESULT Texture::LoadWIC(UINT & index, std::string fileName)
{
	UINT* n = &index;

	std::wstring path = func::ChangeUnicode(fileName.c_str());

	result = DirectX::LoadWICTextureFromFile(dev.lock()->Get(), path.c_str(), &wic[n].con.resource, wic[n].decode, wic[n].sub);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWICテクスチャの読み込み：失敗\n"));
		return result;
	}

	result = CreateHeap(n);
	result = CreateConView(n);
	result = CreateResource(n);

	return result;
}

// 定数バッファヒープの生成
HRESULT Texture::CreateHeap(UINT * index)
{
	//ヒープ設定用構造体の設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = 2;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//ヒープ生成
	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&wic[index].con.heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWICの定数バッファのヒープ生成：失敗\n"));
	}

	return result;
}

// 定数バッファビューの生成
HRESULT Texture::CreateConView(UINT * index)
{
	//シェーダリソースビュー設定用構造体の設定
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = wic[index].con.resource->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//シェーダーリソースビューの生成
	dev.lock()->Get()->CreateShaderResourceView(wic[index].con.resource, &desc, wic[index].con.heap->GetCPUDescriptorHandleForHeapStart());

	return result;
}

// 頂点リソースの生成
HRESULT Texture::CreateResource(UINT* index)
{
	//頂点の数分メモリ確保
	wic[index].vertex.resize(VERTEX_MAX);

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
	desc.Width            = sizeof(Vertex) * wic[index].vertex.size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//リソース生成
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&wic[index].resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWICの頂点リソース生成：失敗\n"));
		return result;
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	result = wic[index].resource->Map(0, &range, reinterpret_cast<void**>(&wic[index].data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWICの頂点のリソースマッピング：失敗\n"));
		return result;
	}

	//頂点データのコピー
	memcpy(wic[index].data, wic[index].vertex.data(), sizeof(Vertex) * wic[index].vertex.size());

	//頂点バッファ設定用構造体の設定
	wic[index].view.BufferLocation = wic[index].resource->GetGPUVirtualAddress();
	wic[index].view.SizeInBytes    = sizeof(Vertex) * wic[index].vertex.size();
	wic[index].view.StrideInBytes  = sizeof(Vertex);

	return result;
}

// 描画
void Texture::Draw(UINT & index, const Vec2f & pos)
{
	UINT* n = &index;

	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = {};
	desc = wic[n].con.resource->GetDesc();

	//左上
	wic[n].vertex[0] = { { pos.x,               pos.y,               0.0f }, { 0.0f, 0.0f } };
	//右上
	wic[n].vertex[1] = { { pos.x + desc.Width,  pos.y,               0.0f }, { 1.0f, 0.0f } };
	//左下
	wic[n].vertex[2] = { { pos.x,               pos.y + desc.Height, 0.0f }, { 0.0f, 1.0f } };
	//右下
	wic[n].vertex[3] = { { pos.x + desc.Width,  pos.y + desc.Height, 0.0f }, { 1.0f, 1.0f } };

	//頂点データの更新
	memcpy(wic[n].data, wic[n].vertex.data(), sizeof(Vertex) * wic[n].vertex.size());

	////頂点バッファ設定用構造体の設定
	//wic[n].view.BufferLocation = wic[n].resource->GetGPUVirtualAddress();
	//wic[n].view.SizeInBytes    = sizeof(Vertex) * wic[n].vertex.size();
	//wic[n].view.StrideInBytes  = sizeof(Vertex);

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &wic[n].view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ボックス設定用構造体の設定
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = desc.Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(desc.Width);
	box.top    = 0;

	//サブリソースに書き込み
	result = wic[n].con.resource->WriteToSubresource(0, &box, wic[n].decode.get(), wic[n].sub.RowPitch, wic[n].sub.SlicePitch);
	if (FAILED(result))
	{
		OutputDebugString(_T("WICのサブリソースへの書き込み：失敗\n"));
		return;
	}

	//ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &wic[n].con.heap);

	//ディスクリプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(2, wic[n].con.heap->GetGPUDescriptorHandleForHeapStart());

	//描画
	list.lock()->GetList()->DrawInstanced(wic[n].vertex.size(), 1, 0, 0);
}

// ユニコード変換
std::wstring func::ChangeUnicode(const CHAR * str)
{
	//文字数の取得
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str, -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//変換
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str, -1, &wstr[0], byteSize);

	return wstr;
}
