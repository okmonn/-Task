#include "Pipe.h"
#include "../Device/Device.h"
#include "../Swap/Swap.h"
#include "../Root/Root.h"
#include "../etc/Release.h"

// �R���X�g���N�^
Pipe::Pipe(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<Root>root) : 
	dev(dev), swap(swap), root(root), pipe(nullptr)
{
}

// �f�X�g���N�^
Pipe::~Pipe()
{
	Release(pipe);
}

// �p�C�v���C���̐���
long Pipe::CreatePipe(const D3D12_INPUT_ELEMENT_DESC & input, const unsigned int & num, const D3D12_PRIMITIVE_TOPOLOGY_TYPE & type, const bool & depth)
{
	//���X�^���C�U�[�X�e�[�g�ݒ�p�\���̂̐ݒ�
	D3D12_RASTERIZER_DESC rasterizer = {};
	rasterizer.AntialiasedLineEnable = FALSE;
	rasterizer.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	rasterizer.CullMode              = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	rasterizer.DepthBias             = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizer.DepthBiasClamp        = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizer.DepthClipEnable       = TRUE;
	rasterizer.FillMode              = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizer.ForcedSampleCount     = 0;
	rasterizer.FrontCounterClockwise = FALSE;
	rasterizer.MultisampleEnable     = FALSE;
	rasterizer.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;

	

	//�����_�[�^�[�Q�b�g�u�����h�ݒ�p�\����
	D3D12_RENDER_TARGET_BLEND_DESC renderBlend = {};
	renderBlend.BlendEnable           = TRUE;
	renderBlend.BlendOp               = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.BlendOpAlpha          = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.DestBlend             = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	renderBlend.DestBlendAlpha        = D3D12_BLEND::D3D12_BLEND_ZERO;
	renderBlend.LogicOp               = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	renderBlend.LogicOpEnable         = FALSE;
	renderBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;
	renderBlend.SrcBlend              = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	renderBlend.SrcBlendAlpha         = D3D12_BLEND::D3D12_BLEND_ONE;

	//�u�����h�X�e�[�g�ݒ�p�\����
	D3D12_BLEND_DESC blend = {};
	blend.AlphaToCoverageEnable  = FALSE;
	blend.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < swap.lock()->GetCnt(); i++)
	{
		blend.RenderTarget[i]    = renderBlend;
	}

	//�p�C�v���C���X�e�[�g�ݒ�p�\����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.BlendState                       = blend;
	desc.DepthStencilState.DepthEnable    = depth;
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.StencilEnable  = FALSE;
	desc.DSVFormat                        = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.InputLayout                      = { &input, num };
	desc.NumRenderTargets                 = 1;
	desc.PrimitiveTopologyType            = type;
	desc.pRootSignature                   = root.lock()->Get();
	desc.PS.pShaderBytecode               = root.lock()->GetPixel()->GetBufferPointer();
	desc.PS.BytecodeLength                = root.lock()->GetPixel()->GetBufferSize();
	desc.RasterizerState                  = rasterizer;
	desc.RTVFormats[0]                    = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleMask                       = UINT_MAX;
	desc.SampleDesc.Count                 = 1;
	desc.VS.pShaderBytecode               = root.lock()->GetVertex()->GetBufferPointer();
	desc.VS.BytecodeLength                = root.lock()->GetVertex()->GetBufferSize();
	
	auto hr = dev.lock()->Get()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�p�C�v���C���̐����F���s\n"));
	}

	return hr;
}
