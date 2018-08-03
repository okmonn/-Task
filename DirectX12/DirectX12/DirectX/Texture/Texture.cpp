#include "Texture.h"
#include "WICTextureLoader12.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include <tchar.h>

// コンストラクタ
Texture::Texture(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap) : 
	win(win), dev(dev), list(list), swap(swap)
{
	wic.clear();


	CoInitialize(nullptr);
}

// デストラクタ
Texture::~Texture()
{
	Liberation();
}

// WIC読み込み
HRESULT Texture::LoadWIC(UINT & index, std::string fileName)
{
	UINT* n = &index;

	std::wstring path = func::ChangeUnicode(fileName.c_str());

	result = DirectX::LoadWICTextureFromFile(dev.lock()->Get(), path.c_str(), &wic[n].resource, wic[n].decode, wic[n].sub);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWICテクスチャの読み込み：失敗\n"));
		return result;
	}

	result = CreateHeap(n);
	result = CreateResourceView(n);
	result = CreateVertex(n);

	return result;
}

// ヒープの生成
HRESULT Texture::CreateHeap(UINT* index)
{
	//ヒープ設定用構造体の設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = swap.lock()->GetBack();
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//ヒープ生成
	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&wic[index].heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nテクスチャの定数バッファ用ヒープの生成：失敗\n"));
	}

	return result;
}

// リソースの生成
HRESULT Texture::CreateResourceView(UINT * index)
{
	//シェーダリソースビュー設定用構造体の設定
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = wic[index].resource->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//ヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle = wic[index].heap->GetCPUDescriptorHandleForHeapStart();

	dev.lock()->Get()->CreateShaderResourceView(wic[index].resource, &desc, handle);

	return result;
}

// 頂点バッファの生成
HRESULT Texture::CreateVertex(UINT * index)
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
	desc.Width            = sizeof(wic[index].v.vertex);//((sizeof(v[index].vertex) + 0xff) &~0xff);
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//リソース生成
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&wic[index].v.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nテクスチャの頂点バッファ用リソースの生成：失敗\n"));
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	result = wic[index].v.resource->Map(0, &range, reinterpret_cast<void**>(&wic[index].v.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWテクスチャの頂点用リソースのマッピング：失敗\n"));
	}

	//頂点データのコピー
	memcpy(wic[index].v.data, &wic[index].v.vertex, sizeof(wic[index].v.vertex));

	//頂点バッファ設定用構造体の設定
	wic[index].v.view.BufferLocation = wic[index].v.resource->GetGPUVirtualAddress();
	wic[index].v.view.SizeInBytes    = sizeof(wic[index].v.vertex);
	wic[index].v.view.StrideInBytes  = sizeof(Vertex);

	return result;
}

// 描画準備
void Texture::SetDraw(UINT * index)
{
	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = {};
	desc = wic[index].resource->GetDesc();

	//ボックス設定用構造体の設定
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = desc.Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(desc.Width);
	box.top    = 0;

	//サブリソースに書き込み
	result = wic[index].resource->WriteToSubresource(0, &box, wic[index].decode.get(), wic[index].sub.RowPitch, wic[index].sub.SlicePitch);
	if (FAILED(result))
	{
		OutputDebugString(_T("テクスチャのサブリソースへの書き込み：失敗\n"));
	}
}

// ヒープのセット
void Texture::SetHeap(UINT * index, UINT table)
{
	//ヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handle = wic[index].heap->GetGPUDescriptorHandleForHeapStart();

	//ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &wic[index].heap);

	//ディスクリプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(table, handle);
}

// 描画
void Texture::Draw(UINT & index)
{
	UINT* n = &index;

	//左上
	wic[n].v.vertex[0] = { {-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f} };
	//右上
	wic[n].v.vertex[1] = { { 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f} };
	//左下
	wic[n].v.vertex[2] = { {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} };
	//右下
	wic[n].v.vertex[3] = { { 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} };

	//頂点データの更新
	memcpy(wic[n].v.data, &wic[n].v.vertex, (sizeof(wic[n].v.vertex)));

	//頂点バッファ設定用構造体の設定
	wic[n].v.view.BufferLocation = wic[n].v.resource->GetGPUVirtualAddress();
	wic[n].v.view.SizeInBytes    = sizeof(wic[n].v.vertex);
	wic[n].v.view.StrideInBytes  = sizeof(Vertex);

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &wic[n].v.view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SetDraw(n);
	SetHeap(n);

	//描画
	list.lock()->GetList()->DrawInstanced(VERTEX_MAX, 1, 0, 0);
}

// 解放
void Texture::Liberation(void)
{
	for (auto itr = wic.begin(); itr != wic.end(); ++itr)
	{
		if (itr->second.v.resource != nullptr)
		{
			itr->second.v.resource->Unmap(0, nullptr);
		}
		Release(itr->second.v.resource);
		itr->second.decode.release();
		Release(itr->second.resource);
		Release(itr->second.heap);
	}
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
