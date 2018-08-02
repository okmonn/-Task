#include "Texture.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include "WICTextureLoader12.h"
#include <tchar.h>

// コンストラクタ
Texture::Texture(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap) : win(win), dev(dev), list(list), swap(swap)
{
	wic.clear();
	CoInitialize(nullptr);
}

// デストラクタ
Texture::~Texture()
{
}

// ヒープの生成
HRESULT Texture::CreateHeap(UINT* index)
{
	//定数バッファ設定用構造体の設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = swap.lock()->GetBack();
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&wic[index].constant.heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nテクスチャ用のヒープ生成：失敗\n"));
	}

	//ヒープサイズを取得
	wic[index].constant.size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(desc.Type);

	return result;
}

// リソースの生成
HRESULT Texture::CreateResourcce(UINT * index)
{
	//ヒープ設定用構造体の設定
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体の設定
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width            = ((sizeof(Image) + 0xff) &~0xff);
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&wic[index].constant.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nテクスチャ用のリソース生成：失敗\n"));
	}

	return result;
}

// バッファビューの生成
HRESULT Texture::CreateView(UINT * index)
{
	//定数バッファビュー設定用構造体の設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = wic[index].constant.resource->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(Image) + 0xff) &~0xff;

	//定数バッファビュー生成
	dev.lock()->Get()->CreateConstantBufferView(&desc, wic[index].constant.heap->GetCPUDescriptorHandleForHeapStart());

	//送信範囲
	D3D12_RANGE range = { 0, 0 };

	//マッピング
	result = wic[index].constant.resource->Map(0, &range, (void**)(&wic[index].constant.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nテクスチャ用のマップ：失敗\n"));
	}

	//コピー
	memcpy(wic[index].constant.data, &wic[index].image, sizeof(Image));

	return result;
}

// シェーダリソースビューの生成
HRESULT Texture::CreateShaderView(UINT * index)
{
	//シェーダリソースビュー設定用構造体の設定
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = wic[index].resource->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//ヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle = wic[index].constant.heap->GetCPUDescriptorHandleForHeapStart();

	//シェーダーリソースビューの生成
	dev.lock()->Get()->CreateShaderResourceView(wic[index].resource, &desc, handle);

	return result;
}

// 頂点リソースの生成
HRESULT Texture::CreateVertexResource(UINT * index)
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
	desc.Width            = sizeof(wic[index].vertex.vertex);//((sizeof(v[index].vertex) + 0xff) &~0xff);
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//リソース生成
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&wic[index].vertex.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nテクスチャの頂点バッファ用リソース生成：失敗\n"));
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	result = wic[index].vertex.resource->Map(0, &range, reinterpret_cast<void**>(&wic[index].vertex.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWIC頂点用マッピング：失敗\n"));
	}

	//頂点データのコピー
	memcpy(wic[index].vertex.data, &wic[index].vertex.vertex, sizeof(wic[index].vertex.vertex));

	//頂点バッファ設定用構造体の設定
	wic[index].vertex.view.BufferLocation = wic[index].vertex.resource->GetGPUVirtualAddress();
	wic[index].vertex.view.SizeInBytes    = sizeof(wic[index].vertex.vertex);
	wic[index].vertex.view.StrideInBytes  = sizeof(Vertex);

	return result;
}

// WIC読み込み
HRESULT Texture::LoadWIC(UINT & index, std::string fileName)
{
	UINT* n = &index;

	std::wstring path = okmonn::ChangeUnicode(fileName.c_str());

	result = DirectX::LoadWICTextureFromFile(dev.lock()->Get(), path.c_str(), &wic[n].resource, wic[n].decode, wic[n].sub);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWICテクスチャの読み込み：失敗\n"));
	}

	wic[n].image.window.x = static_cast<FLOAT>(win.lock()->GetX());
	wic[n].image.window.y = static_cast<FLOAT>(win.lock()->GetY());

	result = CreateHeap(n);
	result = CreateResourcce(n);
	result = CreateView(n);
	result = CreateShaderView(n);
	result = CreateVertexResource(n);

	return result;
}

// ヒープのセット
void Texture::SetHeap(UINT * index, UINT i)
{
	//ヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handle = wic[index].constant.heap->GetGPUDescriptorHandleForHeapStart();

	//ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &wic[index].constant.heap);

	//ディスクリプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(i, handle);
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
	box.right  = (UINT)desc.Width;
	box.top    = 0;

	//サブリソースに書き込み
	result = wic[index].resource->WriteToSubresource(0, &box, wic[index].decode.get(), wic[index].sub.RowPitch, wic[index].sub.SlicePitch);
	if (FAILED(result))
	{
		OutputDebugString(_T("WICサブリソースへの書き込み：失敗\n"));
	}

	wic[index].image.size.x = static_cast<FLOAT>(desc.Width);
	wic[index].image.size.y = static_cast<FLOAT>(desc.Height);
}

void Texture::DrawGraph(UINT & index, FLOAT x, FLOAT y, BOOL turn)
{
	
}

// ユニコード変換
std::wstring okmonn::ChangeUnicode(const CHAR * str)
{
	//文字数の取得
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str, -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//変換
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str, -1, &wstr[0], byteSize);

	return wstr;
}
