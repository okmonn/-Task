#include "Pipe.h"
#include "Device.h"
#include "Swap.h"
#include "RootSignature.h"
#include <tchar.h>

// コンストラクタ
Pipe::Pipe(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<RootSignature>root) : dev(dev), swap(swap), root(root)
{
	pipe = nullptr;


	Create();
}

// デストラクタ
Pipe::~Pipe()
{
	Release(pipe);
}

// パイプラインの生成
HRESULT Pipe::Create(void)
{
	//頂点レイアウト設定用構造体の設定
	D3D12_INPUT_ELEMENT_DESC input[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//ラスタライザーステート設定用構造体の設定
	D3D12_RASTERIZER_DESC rasterizer = {};
	rasterizer.FillMode              = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizer.CullMode              = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	rasterizer.FrontCounterClockwise = FALSE;
	rasterizer.DepthBias             = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizer.DepthBiasClamp        = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizer.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizer.DepthClipEnable       = TRUE;
	rasterizer.MultisampleEnable     = FALSE;
	rasterizer.AntialiasedLineEnable = FALSE;
	rasterizer.ForcedSampleCount     = 0;
	rasterizer.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//レンダーターゲットブレンド設定用構造体
	D3D12_RENDER_TARGET_BLEND_DESC renderBlend = {};
	renderBlend.BlendEnable           = FALSE;
	renderBlend.BlendOp               = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.BlendOpAlpha          = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.DestBlend             = D3D12_BLEND::D3D12_BLEND_ZERO;
	renderBlend.DestBlendAlpha        = D3D12_BLEND::D3D12_BLEND_ZERO;
	renderBlend.LogicOp               = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	renderBlend.LogicOpEnable         = FALSE;
	renderBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;
	renderBlend.SrcBlend              = D3D12_BLEND::D3D12_BLEND_ONE;
	renderBlend.SrcBlendAlpha         = D3D12_BLEND::D3D12_BLEND_ONE;

	//ブレンドステート設定用構造体
	D3D12_BLEND_DESC descBS = {};
	descBS.AlphaToCoverageEnable  = TRUE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < swap.lock()->GetBack(); i++)
	{
		descBS.RenderTarget[i] = renderBlend;
	}

	//パイプラインステート設定用構造体
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.InputLayout                      = { input, _countof(input) };
	desc.PrimitiveTopologyType            = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.pRootSignature                   = root.lock()->Get();
	desc.VS.pShaderBytecode				  = root.lock()->GetVertex()->GetBufferPointer();
	desc.VS.BytecodeLength				  = root.lock()->GetVertex()->GetBufferSize();
	desc.PS.pShaderBytecode				  = root.lock()->GetPixel()->GetBufferPointer();
	desc.PS.BytecodeLength				  = root.lock()->GetPixel()->GetBufferSize();
	desc.RasterizerState                  = rasterizer;
	desc.BlendState                       = descBS;
	desc.DepthStencilState.DepthEnable    = true;
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.StencilEnable  = FALSE;
	desc.SampleMask                       = UINT_MAX;
	desc.NumRenderTargets                 = 1;
	desc.RTVFormats[0]                    = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DSVFormat                        = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count                 = 1;

	//パイプライン生成
	result = dev.lock()->Get()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nパイプラインの生成：失敗\n"));
	}

	return result;
}
