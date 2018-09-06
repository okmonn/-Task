#include "Texture.h"
#include "WICTextureLoader12.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Root.h"
#include "../PipeLine/Pipe.h"
#include "../Descriptor/Constant.h"
#include <tchar.h>

// 頂点の最大数
#define VERTEX_MAX 4

// コンストラクタ
Texture::Texture(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<Constant>con) :
	root(root), pipe(pipe), con(con)
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
HRESULT Texture::LoadWIC(UINT & index, const std::string& fileName)
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
	result = CreateShaderView(n);
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
	desc.NumDescriptors = 1;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//ヒープ生成
	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&wic[index].con.heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nテクスチャの定数ヒープの生成：失敗\n"));
	}

	return result;
}

// シェーダリソースビューの生成
HRESULT Texture::CreateShaderView(UINT * index)
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
	//配列のメモリ確保
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
	desc.Width            = sizeof(TexVertex) * wic[index].vertex.size();
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
		OutputDebugString(_T("\nテクスチャの頂点リソースの生成：失敗\n"));
		return result;
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	result = wic[index].resource->Map(0, &range, reinterpret_cast<void**>(&wic[index].data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nテクスチャの頂点マッピング：失敗\n"));
		return result;
	}

	//頂点データのコピー
	memcpy(wic[index].data, wic[index].vertex.data(), sizeof(TexVertex) * wic[index].vertex.size());

	//頂点バッファ設定用構造体の設定
	wic[index].view.BufferLocation = wic[index].resource->GetGPUVirtualAddress();
	wic[index].view.SizeInBytes    = sizeof(TexVertex) * wic[index].vertex.size();
	wic[index].view.StrideInBytes  = sizeof(TexVertex);

	return result;
}

// 描画準備
HRESULT Texture::SetDraw(UINT & index)
{
	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = wic[&index].con.resource->GetDesc();

	//ボックス設定用構造体の設定
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = desc.Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(desc.Width);
	box.top    = 0;

	//サブリソースに書き込み
	result = wic[&index].con.resource->WriteToSubresource(0, &box, wic[&index].decode.get(), wic[&index].sub.RowPitch, wic[&index].sub.SlicePitch);
	if (FAILED(result))
	{
		OutputDebugString(_T("テクスチャのサブリソース書込み：失敗\n"));
		return result;
	}

	SetDescriptor(index);

	return result;
}

// ディスクリプターのセット
void Texture::SetDescriptor(UINT& index)
{
	//ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &wic[&index].con.heap);

	//ディスクリプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(1, wic[&index].con.heap->GetGPUDescriptorHandleForHeapStart());
}

// 描画
void Texture::Draw(UINT & index, const Vec2f & pos, float alpha, UINT turnX, UINT turnY)
{
	UINT* n = &index;

	list.lock()->GetList()->SetGraphicsRootSignature(root.lock()->Get());
	list.lock()->GetList()->SetPipelineState(pipe.lock()->Get());

	con.lock()->SetConstant();

	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = wic[n].con.resource->GetDesc();

	//UV座標
	DirectX::XMFLOAT2 leftUp    = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 rightUp   = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 leftDown  = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };
	DirectX::XMFLOAT2 rightDown = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };

	//左上
	wic[n].vertex[0] = { { pos.x,                                   pos.y,                                   0.0f }, leftUp,    alpha };
	//右上
	wic[n].vertex[1] = { { pos.x + static_cast<FLOAT>(desc.Width),  pos.y,                                   0.0f }, rightUp,   alpha };
	//左下
	wic[n].vertex[2] = { { pos.x,                                   pos.y + static_cast<FLOAT>(desc.Height), 0.0f }, leftDown,  alpha };
	//右下
	wic[n].vertex[3] = { { pos.x + static_cast<FLOAT>(desc.Width),  pos.y + static_cast<FLOAT>(desc.Height), 0.0f }, rightDown, alpha };

	//頂点データの更新
	memcpy(wic[n].data, wic[n].vertex.data(), sizeof(TexVertex) * wic[n].vertex.size());

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &wic[n].view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (FAILED(SetDraw(index)))
	{
		return;
	}

	//描画
	list.lock()->GetList()->DrawInstanced(wic[n].vertex.size(), 1, 0, 0);
}

// 描画・サイズ指定
void Texture::Draw(UINT & index, const Vec2f & pos, const Vec2f & size, float alpha, UINT turnX, UINT turnY)
{
	UINT* n = &index;

	list.lock()->GetList()->SetGraphicsRootSignature(root.lock()->Get());
	list.lock()->GetList()->SetPipelineState(pipe.lock()->Get());

	con.lock()->SetConstant();

	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = wic[n].con.resource->GetDesc();

	//UV座標
	DirectX::XMFLOAT2 leftUp    = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 rightUp   = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) * turnY) };
	DirectX::XMFLOAT2 leftDown  = { (static_cast<FLOAT>(desc.Width) * turnX),                                    (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };
	DirectX::XMFLOAT2 rightDown = { (static_cast<FLOAT>(desc.Width) - (static_cast<FLOAT>(desc.Width) * turnX)), (static_cast<FLOAT>(desc.Height) - (static_cast<FLOAT>(desc.Height) * turnY)) };

	//左上
	wic[n].vertex[0] = { { pos.x,           pos.y,          0.0f }, leftUp,   alpha };
	//右上
	wic[n].vertex[1] = { { pos.x + size.x,  pos.y,          0.0f }, rightUp,  alpha };
	//左下
	wic[n].vertex[2] = { { pos.x,           pos.y + size.y, 0.0f }, leftDown, alpha };
	//右下
	wic[n].vertex[3] = { { pos.x + size.x,  pos.y + size.y, 0.0f }, rightDown,alpha };

	//頂点データの更新
	memcpy(wic[n].data, wic[n].vertex.data(), sizeof(TexVertex) * wic[n].vertex.size());

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &wic[n].view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (FAILED(SetDraw(index)))
	{
		return;
	}

	//描画
	list.lock()->GetList()->DrawInstanced(wic[n].vertex.size(), 1, 0, 0);
}

// 描画・サイズ指定・分割
void Texture::Draw(UINT& index, const Vec2f& pos, const Vec2f& size, const Vec2f& rectPos, const Vec2f& rectSize, float alpha, UINT turnX, UINT turnY)
{
	UINT* n = &index;

	list.lock()->GetList()->SetGraphicsRootSignature(root.lock()->Get());
	list.lock()->GetList()->SetPipelineState(pipe.lock()->Get());

	con.lock()->SetConstant();

	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = wic[n].con.resource->GetDesc();

	//UV座標
	DirectX::XMFLOAT2 leftUp    = { rectPos.x + (rectSize.x * turnX),                rectPos.y + (rectSize.y * turnY) };
	DirectX::XMFLOAT2 rightUp   = { rectPos.x + (rectSize.x - (rectSize.x * turnX)), rectPos.y + (rectSize.y * turnY) };
	DirectX::XMFLOAT2 leftDown  = { rectPos.x + (rectSize.x * turnX),                rectPos.y + (rectSize.y - (rectSize.y * turnY)) };
	DirectX::XMFLOAT2 rightDown = { rectPos.x + (rectSize.x - (rectSize.x * turnX)), rectPos.y + (rectSize.y - (rectSize.y * turnY)) };

	//左上
	wic[n].vertex[0] = { { pos.x,           pos.y,          0.0f }, leftUp,    alpha };
	//右上
	wic[n].vertex[1] = { { pos.x + size.x,  pos.y,          0.0f }, rightUp,   alpha };
	//左下
	wic[n].vertex[2] = { { pos.x,           pos.y + size.y, 0.0f }, leftDown,  alpha };
	//右下
	wic[n].vertex[3] = { { pos.x + size.x,  pos.y + size.y, 0.0f }, rightDown, alpha };

	//頂点データの更新
	memcpy(wic[n].data, wic[n].vertex.data(), sizeof(TexVertex) * wic[n].vertex.size());

	//頂点バッファビューのセット
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &wic[n].view);

	//トポロジー設定
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (FAILED(SetDraw(index)))
	{
		return;
	}

	//描画
	list.lock()->GetList()->DrawInstanced(wic[n].vertex.size(), 1, 0, 0);
}

// 削除
void Texture::Delete(UINT & index)
{
	if (wic.find(&index) != wic.end())
	{
		if (wic[&index].resource != nullptr)
		{
			wic[&index].resource->Unmap(0, nullptr);
			wic[&index].decode.release();
			Release(wic[&index].resource);
			Release(wic[&index].con.resource);
			Release(wic[&index].con.heap);
		}

		wic.erase(wic.find(&index));
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
