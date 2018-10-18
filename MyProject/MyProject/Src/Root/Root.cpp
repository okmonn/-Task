#include <d3dcompiler.h>
#include "Root.h"
#include "../Device/Device.h"
#include "../etc/Release.h"

#pragma comment (lib, "d3dcompiler.lib")

// コンストラクタ
Root::Root(std::weak_ptr<Device>dev, const std::wstring& fileName) : 
	dev(dev), root(nullptr), signature(nullptr), error(nullptr), vertex(nullptr), pixel(nullptr)
{
	ShaderCompile(fileName);
	Create();
}

// デストラクタ
Root::~Root()
{
	Release(pixel);
	Release(vertex);
	Release(signature);
	Release(error);
	Release(root);
}

// シェーダのコンパイル
long Root::ShaderCompile(const std::wstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "VS", "vs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex, &error);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n頂点シェーダの読み込み：失敗\n"));
		return hr;
	}

	hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "PS", "ps_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel, &error);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nピクセルシェーダの読み込み：失敗\n"));
		return hr;
	}

	//シェーダからルートシグネチャの情報を取得
	hr = D3DGetBlobPart(vertex->GetBufferPointer(), vertex->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &signature);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nルートシグネチャ情報の取得：失敗\n"));
	}

	return 0;
}

// ルートシグネチャの生成
long Root::Create(void)
{
	auto hr = dev.lock()->Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nルートシグネチャの生成：失敗\n"));
	}

	return hr;
}
