//ルートシグネチャの宣言
//#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),\
//CBV(b0),\
//SRV(t0),\
//DescriptorTable(CBV(b1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),\
//DescriptorTable(SRV(t1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),\
//StaticSampler(s0, filter = FILTER_MIN_MAG_MIP_LINEAR,\
//addressU = TEXTURE_ADDRESS_WRAP,\
//addressV = TEXTURE_ADDRESS_WRAP,\
//addressW = TEXTURE_ADDRESS_WRAP,\
//mipLodBias = 0.0f,\
//maxAnisotropy = 0,\
//comparisonFunc = COMPARISON_NEVER,\
//borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,\
//minLOD = 0.0f,\
//maxLOD = 3.402823466e+38f,\
//space = 0,\
//visibility = SHADER_VISIBILITY_ALL)"

#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),\
CBV(b0, space = 0, flags = DATA_STATIC),\
DescriptorTable(SRV(t0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),\
StaticSampler(s0, filter = FILTER_MIN_MAG_MIP_LINEAR,\
addressU = TEXTURE_ADDRESS_WRAP,\
addressV = TEXTURE_ADDRESS_WRAP,\
addressW = TEXTURE_ADDRESS_WRAP,\
mipLodBias = 0.0f,\
maxAnisotropy = 16,\
comparisonFunc = COMPARISON_NEVER,\
borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,\
minLOD = 0.0f,\
maxLOD = 3.402823466e+38f,\
space = 0,\
visibility = SHADER_VISIBILITY_ALL)"

//テクスチャ
Texture2D<float4> tex : register(t0);
//サンプラー
SamplerState smp : register(s0);

//ワールドビュープロジェクション
cbuffer wvp : register(b0)
{
	//ワールド行列
    float4x4 world;
	//ビュープロジェクション行列
    float4x4 viewProjection;
}

//ウィンドウサイズ・画像サイズ
cbuffer size : register(b1)
{
	//ウィンドウサイズ
	float2 window;
	//画像サイズ
	float2 imageSize;
	//画像UV
	float2 imageUv;
}

//出力
struct Out
{
	//システム座標
    float4 svpos : SV_POSITION;
	//座標
    float4 pos : POSITION;
	//uv値
    float2 uv : TEXCOORD;
};

//入力
struct VSInput
{
	//座標
    float4 pos : POSITION;
	//uv
    float2 uv : TEXCOORD;
};

//頂点シェーダ
[RootSignature(RS)]
Out BasicVS(VSInput input)
{
	/*座標補正
	pos.xy = float2(-1, 1) + (pos.xy / float2((640 / 2), -(480 / 2)));*/

    Out o;
    o.svpos = input.pos;
    o.pos = input.pos;
    o.uv = input.uv;
	
    return o;
}

//ピクセルシェーダ
[RootSignature(RS)]
float4 BasicPS(Out o) : SV_TARGET
{
	//return float4(tex.Sample(smp, o.uv).rgb, 0);
    float4 t = tex.Sample(smp, o.uv);
    /*if (t.a <= 0.0)
    {
        discard;
    }
*/
	return float4(1, 1, 1, 1);
    return t;
}