#include <d3dcompiler.h>
#include "RootSignature.h"
#include "Device.h"
#include <tchar.h>

#pragma comment (lib, "d3dcompiler.lib")

// �R���X�g���N�^
RootSignature::RootSignature(std::weak_ptr<Device>dev) : dev(dev), rootSignature(nullptr)
{
	signature = nullptr;
	error = nullptr;
	vertex = nullptr;
	pixel = nullptr;


	Serialize();
	Create();
}

// �R���X�g���N�^
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

// �f�X�g���N�^
RootSignature::~RootSignature()
{
	Release(error);
	Release(signature);
	Release(rootSignature);
}

// ���_�V�F�[�_�̃R���p�C��
HRESULT RootSignature::ComVer(LPCWSTR fileName, LPCSTR func, LPCSTR target)
{
	result = D3DCompileFromFile(fileName, nullptr, nullptr, func, target, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex, &error);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n���_�V�F�[�_�̃R���p�C���F���s\n"));
	}

	return result;
}

// �s�N�Z���V�F�[�_�̃R���p�C��
HRESULT RootSignature::ComPix(LPCWSTR fileName, LPCSTR func, LPCSTR target)
{
	result = D3DCompileFromFile(fileName, nullptr, nullptr, func, target, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel, &error);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�s�N�Z���V�F�[�_�̃R���p�C���F���s\n"));
	}

	return result;
}

// �V���A���C�Y
HRESULT RootSignature::Serialize(void)
{
	//�f�B�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE range[3];
	SecureZeroMemory(&range, sizeof(range));

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER param[3];
	SecureZeroMemory(&param, sizeof(param));

	//�萔�o�b�t�@�p
	range[0].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range[0].NumDescriptors                    = 1;
	range[0].BaseShaderRegister                = 0;
	range[0].RegisterSpace                     = 0;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	param[0].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[0].ShaderVisibility                    = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
	param[0].DescriptorTable.NumDescriptorRanges = 1;
	param[0].DescriptorTable.pDescriptorRanges   = &range[0];

	//�萔�o�b�t�@�p
	range[1].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range[1].NumDescriptors                    = 1;
	range[1].BaseShaderRegister                = 1;
	range[1].RegisterSpace                     = 0;
	range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	param[1].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[1].ShaderVisibility                    = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
	param[1].DescriptorTable.NumDescriptorRanges = 1;
	param[1].DescriptorTable.pDescriptorRanges   = &range[1];

	//�e�N�X�`���p
	range[2].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[2].NumDescriptors                    = 1;
	range[2].BaseShaderRegister                = 0;
	range[2].RegisterSpace                     = 0;
	range[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	param[2].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[2].ShaderVisibility                    = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
	param[2].DescriptorTable.NumDescriptorRanges = 1;
	param[2].DescriptorTable.pDescriptorRanges   = &range[2];


	//�ÓI�T���v���[�̐ݒ�
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

	//���[�g�V�O�l�`���ݒ�p�\���̂̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC desc = {};
	desc.NumParameters     = _countof(param);
	desc.pParameters       = param;
	desc.NumStaticSamplers = 1;
	desc.pStaticSamplers   = &sampler;
	desc.Flags             = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//���[�g�V�O�l�`���̃V���A���C�Y��
	result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�V���A���C�Y�F���s\n"));
	}

	return result;
}

// ���[�g�V�O�l�`���̐���
HRESULT RootSignature::Create(void)
{
	result = dev.lock()->Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n���[�g�V�O�l�`���̐����F���s\n"));
	}

	return result;
}

// ���[�g�V�O�l�`���̐���
HRESULT RootSignature::CreateSignature(void)
{
	//�V�F�[�_����̃t�F�b�`
	result = D3DGetBlobPart(vertex->GetBufferPointer(), vertex->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &signature);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�V�F�[�_����̃t�F�b�`�F���s\n"));
	}

	result = dev.lock()->Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n���[�g�V�O�l�`���̐����F���s\n"));
	}

	return result;
}
