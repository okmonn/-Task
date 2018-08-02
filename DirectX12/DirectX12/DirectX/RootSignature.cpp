#include <d3dcompiler.h>
#include "RootSignature.h"
#include "Device.h"
#include <tchar.h>

#pragma comment (lib, "d3dcompiler.lib")

// コンストラクタ
RootSignature::RootSignature(std::weak_ptr<Device>dev) : dev(dev), rootSignature(nullptr)
{
	signature = nullptr;
	error = nullptr;
	vertex = nullptr;
	pixel = nullptr;


	Serialize();
	Create();
}

// コンストラクタ
RootSignature::RootSignature(std::weak_ptr<Device>dev, LPCWSTR fileName, LPCSTR func1, LPCSTR func2) : dev(dev), rootSignature(nullptr)
{
	signature = nullptr;
	error = nullptr;
	vertex = nullptr;
	pixel = nullptr;


	ComVer(fileName, func1);
	ComPix(fileName, func2);
	CreateSignature();
}

// デストラクタ
RootSignature::~RootSignature()
{
	Release(error);
	Release(signature);
	Release(rootSignature);
}

// 頂点シェーダのコンパイル
HRESULT RootSignature::ComVer(LPCWSTR fileName, LPCSTR func, LPCSTR target)
{
	result = D3DCompileFromFile(fileName, nullptr, nullptr, func, target, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex, &error);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n頂点シェーダのコンパイル：失敗\n"));
	}

	return result;
}

// ピクセルシェーダのコンパイル
HRESULT RootSignature::ComPix(LPCWSTR fileName, LPCSTR func, LPCSTR target)
{
	result = D3DCompileFromFile(fileName, nullptr, nullptr, func, target, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel, &error);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nピクセルシェーダのコンパイル：失敗\n"));
	}

	return result;
}

// シリアライズ
HRESULT RootSignature::Serialize(void)
{
	//ディスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE range[3];
	SecureZeroMemory(&range, sizeof(range));

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER param[3];
	SecureZeroMemory(&param, sizeof(param));

	//定数バッファ用
	range[0].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range[0].NumDescriptors                    = 1;
	range[0].BaseShaderRegister                = 0;
	range[0].RegisterSpace                     = 0;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	param[0].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[0].ShaderVisibility                    = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
	param[0].DescriptorTable.NumDescriptorRanges = 1;
	param[0].DescriptorTable.pDescriptorRanges   = &range[0];

	//定数バッファ用
	range[1].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range[1].NumDescriptors                    = 1;
	range[1].BaseShaderRegister                = 1;
	range[1].RegisterSpace                     = 0;
	range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	param[1].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[1].ShaderVisibility                    = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
	param[1].DescriptorTable.NumDescriptorRanges = 1;
	param[1].DescriptorTable.pDescriptorRanges   = &range[1];

	//テクスチャ用
	range[2].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[2].NumDescriptors                    = 1;
	range[2].BaseShaderRegister                = 0;
	range[2].RegisterSpace                     = 0;
	range[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	param[2].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[2].ShaderVisibility                    = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
	param[2].DescriptorTable.NumDescriptorRanges = 1;
	param[2].DescriptorTable.pDescriptorRanges   = &range[2];


	//静的サンプラーの設定
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter           = D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampler.AddressU         = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV         = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW         = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias       = 0;
	sampler.MaxAnisotropy    = 0;
	sampler.ComparisonFunc   = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor      = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD           = 0.0f;
	sampler.MaxLOD           = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister   = 0;
	sampler.RegisterSpace    = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

	//ルートシグネチャ設定用構造体の設定
	D3D12_ROOT_SIGNATURE_DESC desc = {};
	desc.NumParameters     = _countof(param);
	desc.pParameters       = param;
	desc.NumStaticSamplers = 1;
	desc.pStaticSamplers   = &sampler;
	desc.Flags             = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートシグネチャのシリアライズ化
	result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nシリアライズ：失敗\n"));
	}

	return result;
}

// ルートシグネチャの生成
HRESULT RootSignature::Create(void)
{
	result = dev.lock()->Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nルートシグネチャの生成：失敗\n"));
	}

	return result;
}

// ルートシグネチャの生成
HRESULT RootSignature::CreateSignature(void)
{
	//シェーダからのフェッチ
	result = D3DGetBlobPart(vertex->GetBufferPointer(), vertex->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &signature);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nシェーダからのフェッチ：失敗\n"));
	}

	result = dev.lock()->Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nルートシグネチャの生成：失敗\n"));
	}

	return result;
}
