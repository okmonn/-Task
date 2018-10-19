#include "LoadTbl.h"
#include "TextureLoader.h"
#include "../Device/Device.h"
#include "../Func/Func.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

#pragma comment(lib, "DirectXTex.lib")

// コンストラクタ
TextureLoader::TextureLoader(std::weak_ptr<Device>dev) :
	dev(dev)
{
	origin.clear();
}

// デストラクタ
TextureLoader::~TextureLoader()
{
	for (auto itr = origin.begin(); itr != origin.end(); ++itr)
	{
		Release(itr->second.rsc);
		Release(itr->second.img);
		Delete(itr->second.img);
		Delete(itr->second.meta);
	}
}

// 読み込み
long TextureLoader::Load(const std::string & fileName, ID3D12Resource * rsc, DirectX::TexMetadata * meta, DirectX::ScratchImage * img)
{
	HRESULT hr = S_OK;

	if (origin.find(fileName) == origin.end())
	{
		auto pos = fileName.find('.');
		std::string fmt = fileName.substr(pos + 1, fileName.size());

		origin[fileName].meta = new DirectX::TexMetadata({});
		origin[fileName].img  = new DirectX::ScratchImage();

		hr = tex::loadTbl[fmt](func::ChangeWString(fileName), origin[fileName].meta, origin[fileName].img);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nファイルの参照：失敗\n"));
			return hr;
		}

		hr = CreateRsc(fileName);
	}

	//アドレスのコピー
	&rsc  = &origin[fileName].rsc;
	meta = &origin[fileName].meta;
	img  = &origin[fileName].img;

	return hr;
}

// 定数リソースの生成
long TextureLoader::CreateRsc(const std::string& fileName)
{
	//プロパティ設定用構造体の設定
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体の設定
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment          = 0;
	desc.DepthOrArraySize   = 1;
	desc.Dimension          = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags              = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height             = origin[fileName].meta->height;
	desc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels          = 0;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width              = origin[fileName].meta->width;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&origin[fileName].rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nテクスチャ用定数リソースの生成：失敗\n"));
	}

	return hr;
}

