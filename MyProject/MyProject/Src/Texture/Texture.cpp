#include <DirectXTex.h>
#include "Texture.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "TextureLoader.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// 頂点最大数
#define MAX 4

// コンストラクタ
Texture::Texture(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, 
	std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<TextureLoader>loader) :
	dev(dev), list(list), con(con), root(root), pipe(pipe), loader(loader)
{
	tex.clear();
}

// デストラクタ
Texture::~Texture()
{
	for (auto itr = tex.begin(); itr != tex.end(); ++itr)
	{
		UnMap(itr->second.v_rsc);
		Release(itr->second.v_rsc);
		Release(itr->second.heap);
	}
}

// ヒープの生成
long Texture::CreateHeap(int * i)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = 1;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&tex[i].heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("テクスチャ用ヒープの生成：失敗\n"));
	}

	return hr;
}

// リソースビューの生成
void Texture::CreateView(int * i)
{
	//シェーダリソースビュー設定用構造体
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	dev.lock()->Get()->CreateShaderResourceView(tex[i].c_rsc, &desc, tex[i].heap->GetCPUDescriptorHandleForHeapStart());
}

// 頂点リソースの生成
long Texture::CreateRsc(int * i)
{
	//配列のメモリ確保
	tex[i].vertex.resize(MAX);

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
	desc.Width              = sizeof(tex::Vertex) * MAX;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&tex[i].v_rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nポイント用頂点リソースの生成：失敗\n"));
	}

	return hr;
}

// マップ
long Texture::Map(int * i)
{
	auto hr = tex[i].v_rsc->Map(0, nullptr, (void**)(tex[i].data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nテクスチャの頂点マッピング：失敗\n"));
		return hr;
	}
	//頂点データのコピー
	memcpy(tex[i].data, tex[i].vertex.data(), sizeof(tex::Vertex) * tex[i].vertex.size());

	return 0;
}

// 読み込み
long Texture::Load(const std::string & fileName, int & i)
{
	auto hr = loader.lock()->Load(fileName, tex[&i].c_rsc, tex[&i].meta, tex[&i].img);
	if (FAILED(hr))
	{
		return hr;
	}

	CreateHeap(&i);
	CreateView(&i);
	CreateRsc(&i);
	Map(&i);

	return hr;
}

// 描画準備
long Texture::SetDraw(int & i, const unsigned int & rootNum)
{
	//ボックス設定用構造体の設定
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = tex[&i].meta->height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(tex[&i].meta->width);
	box.top    = 0;

	//サブリソースに書き込み
	auto hr = tex[&i].c_rsc->WriteToSubresource(0, &box, tex[&i].img->GetPixels(), tex[&i].meta->width * 4, tex[&i].meta->height * 4);
	if (FAILED(hr))
	{
		OutputDebugString(_T("テクスチャのサブリソース書込み：失敗\n"));
		return hr;
	}

	SetHeap(i, rootNum);

	return hr;
}

// ヒープのセット
void Texture::SetHeap(int & i, const unsigned int & rootNum)
{
	//ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &tex[&i].heap);

	//ディスクラプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(rootNum, tex[&i].heap->GetGPUDescriptorHandleForHeapStart());
}

// 描画
void Texture::Draw(int & i, const float & x, const float & y, const float & alpha, const int & turnX, const int & turnY)
{
	int* n = &i;

	//コマンドリストへのセット
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	//UV座標
	DirectX::XMFLOAT2 leftUp    = { (static_cast<FLOAT>(tex[n].meta->width) * turnX),                                            (static_cast<FLOAT>(tex[n].meta->height) * turnY) };
	DirectX::XMFLOAT2 rightUp   = { (static_cast<FLOAT>(tex[n].meta->width) - (static_cast<FLOAT>(tex[n].meta->width) * turnX)), (static_cast<FLOAT>(tex[n].meta->height) * turnY) };
	DirectX::XMFLOAT2 leftDown  = { (static_cast<FLOAT>(tex[n].meta->width) * turnX),                                            (static_cast<FLOAT>(tex[n].meta->height) - (static_cast<FLOAT>(tex[n].meta->height) * turnY)) };
	DirectX::XMFLOAT2 rightDown = { (static_cast<FLOAT>(tex[n].meta->width) - (static_cast<FLOAT>(tex[n].meta->width) * turnX)), (static_cast<FLOAT>(tex[n].meta->height) - (static_cast<FLOAT>(tex[n].meta->height) * turnY)) };

	//左上
	tex[n].vertex[0] = { { x,                                          y,                                           0.0f }, leftUp,    alpha };
	//右上
	tex[n].vertex[1] = { { x + static_cast<FLOAT>(tex[n].meta->width), y,                                           0.0f }, rightUp,   alpha };
	//左下
	tex[n].vertex[2] = { { x,                                          y + static_cast<FLOAT>(tex[n].meta->height), 0.0f }, leftDown,  alpha };
	//右下
	tex[n].vertex[3] = { { x + static_cast<FLOAT>(tex[n].meta->width), y + static_cast<FLOAT>(tex[n].meta->height), 0.0f }, rightDown, alpha };

	//頂点データの更新
	memcpy(tex[n].data, tex[n].vertex.data(), sizeof(tex::Vertex) * tex[n].vertex.size());

	//頂点バッファ設定用構造体の設定
	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = tex[n].v_rsc->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(tex::Vertex) * tex[n].vertex.size();
	view.StrideInBytes  = sizeof(tex::Vertex);

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (FAILED(SetDraw(i)))
	{
		return;
	}

	//描画
	list.lock()->GetList()->DrawInstanced(tex[n].vertex.size(), 1, 0, 0);
}
