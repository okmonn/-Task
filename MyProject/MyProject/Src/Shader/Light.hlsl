// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_VERTEX),"\
                    "DescriptorTable(CBV(b1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_VERTEX),"\
                    "StaticSampler(s0, "\
                                  "filter = FILTER_MIN_MAG_MIP_LINEAR, "\
                                  "addressU = TEXTURE_ADDRESS_CLAMP, "\
                                  "addressV = TEXTURE_ADDRESS_CLAMP, "\
                                  "addressW = TEXTURE_ADDRESS_CLAMP, "\
                                  "mipLodBias = 0.0f, "\
                                  "maxAnisotropy = 0, "\
                                  "comparisonFunc = COMPARISON_NEVER, "\
                                  "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
                                  "minLOD = 0.0f, "\
                                  "maxLOD = 3.402823466e+38f, "\
                                  "space = 0, "\
                                  "visibility = SHADER_VISIBILITY_ALL)"

SamplerState smp : register(s0);

//ワールドビュープロジェクション
cbuffer wvp : register(b0)
{
	//ワールド行列
    float4x4 world;
	//ビュー行列
    float4x4 view;
    //プロジェクション行列
    float4x4 projection;
    float4x4 light;
    float4 eye;
     //ウィンドウサイズ
    float2 window;
}

//ボーン
cbuffer born : register(b1)
{
    //ボーン
    matrix borns[256];
}

struct Out
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
};

//入力
struct VSInput
{
    //座標
    float4 pos : POSITION;
     //ボーン
    min16uint2 born : BORN;
    //ウェイト
    min16uint weight : WEIGHT;
};

//頂点シェーダ
[RootSignature(RS)]
Out VS(VSInput input)
{
    float w = input.weight / 100.0f;
    matrix m = borns[input.born.x] * w + borns[input.born.y] * (1.0f - w);
    m = mul(light, world);

    input.pos = mul(m, input.pos);

    Out o;
    o.svpos = mul(mul(light, world), input.pos);
    o.pos = input.pos;
    return o;
}

//ピクセルシェーダ
[RootSignature(RS)]
float4 PS(Out o) : SV_TARGET
{
    return float4(1, 1, 1, 1);

}