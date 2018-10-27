#include "TextureLoader.h"
#include "WICTextureLoader12.h"
#include "../Device/Device.h"
#include "../Func/Func.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

// 頂点最大数
#define MAX 4

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
		itr->second.decode.release();
		itr->second.sub.reset();
		Release(itr->second.rsc);
		Release(itr->second.heap);
	}
}

// ヒープの生成
long TextureLoader::CreateHeap(const std::string & fileName)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = 1;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&origin[fileName].heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("テクスチャ用ヒープの生成：失敗\n"));
	}

	return hr;
}

// 読み込み
long TextureLoader::Load(const std::string & fileName)
{
	HRESULT hr = S_OK;

	if (origin.find(fileName) == origin.end())
	{
		auto path = func::ChangeWString(fileName);

		origin[fileName].sub = std::make_shared<D3D12_SUBRESOURCE_DATA>();

		hr = DirectX::LoadWICTextureFromFile(dev.lock()->Get(), path.c_str(), &origin[fileName].rsc, origin[fileName].decode, *origin[fileName].sub);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nファイルの参照：失敗\n"));
		}

		CreateHeap(fileName);
	}

	return hr;
}
