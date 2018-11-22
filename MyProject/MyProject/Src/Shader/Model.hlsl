// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_VERTEX),"\
                    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b2, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_VERTEX),"\
                    "DescriptorTable(SRV(t1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t2, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_PIXEL),"\
                    "DescriptorTable(SRV(t3, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_PIXEL),"\
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

Texture2D<float4> tex : register(t0);
Texture2D<float4> sph : register(t1);
Texture2D<float4> spa : register(t2);
Texture2D<float4> ton : register(t3);

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

//マテリアル
cbuffer mat : register(b1)
{
    //基本色
    float3 diffuse;
    //透明度
    float alpha;
    //反射強度
    float specularity;
	//反射色
    float3 specula;
    //環境色
    float3 mirror;
    //乗算テクスチャ対応フラグ
    bool sphFlag;
    //加算テクスチャ対応フラグ
    bool spaFlag;
    //テクスチャ対応フラグ
    bool texFlag;
}

//ボーン
cbuffer born : register(b2)
{
    //ボーン
    matrix borns[256];
}

//出力
struct Out
{
    //システム座標
    float4 svpos     : SV_POSITION;
    //座標
    float4 pos       : POSITION;
    //法線
    float3 normal    : NORMAL;
    //uv
    float2 uv        : TEXCOORD;
    //ボーン
    min16uint2 born  : BORN;
     //ウェイト
    min16uint weight : WEIGHT;
};

//入力
struct VSInput
{
    //座標
    float4 pos       : POSITION;
    //法線
    float4 normal    : NORMAL;
    //uv
    float2 uv        : TEXCOORD;
    //ボーン
    min16uint2 born  : BORN;
    //ウェイト
    min16uint weight : WEIGHT;
};

//頂点シェーダ
[RootSignature(RS)]
Out VS(VSInput input)
{
    float w  = input.weight / 100.0f;
    matrix m = borns[input.born.x] * w + borns[input.born.y] * (1.0f - w);

    m = mul(world, m);
    Out o;
    o.svpos = mul(m, input.pos);
 
    m = mul(view, m);
    m = mul(projection, m);
    input.pos = mul(m, input.pos);
    o.pos    = input.pos;
    o.normal = mul(world, input.normal);
    o.uv     = input.uv;
    o.born   = input.born;
    o.weight = input.weight;

    return o;
}

//ピクセルシェーダ
[RootSignature(RS)]
float4 PS(Out o) : SV_TARGET
{
    //ライトの色
    float3 lightCol = float3(0.6f, 0.6f, 0.6f);

    //視線ベクトル
    float3 eye = float3(0.0f, 10.0f, -15.0f);
    float3 ray = normalize(o.pos.xyz - eye);

    //光源ベクトル
    float3 light = normalize(float3(-1.0f, 1.0f, -1.0f));

    //反射ベクトル
    float3 ref = reflect(-light, o.normal);

    //スぺキュラ
    float spec = pow(saturate(dot(ref, ray)), specularity);

    //光源ベクトルと法線との内積
    float bright = saturate(dot(light, o.normal));

    //ランバート
    bright = saturate(acos(bright) / 3.14159265f);

    float4 toon = ton.Sample(smp, float2(0.0f, 1.0f - bright));

    //色
    float3 color = (texFlag == true ? tex.Sample(smp, o.uv).rgb : saturate(diffuse * bright + specula * spec + mirror * lightCol));
    color = (sphFlag == true ? color * sph.Sample(smp, o.uv).rgb : color);
    color = (spaFlag == true ? color + spa.Sample(smp, o.uv).rgb : color);

    return float4(color, alpha);
}