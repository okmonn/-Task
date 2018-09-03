Texture2D<float4> tex : register(t0);
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
     //ウィンドウサイズ
    float2 window;
}

//マテリアル
cbuffer mat : register(b1)
{
    //基本色
    float3 diffuse;
    //テクスチャ対応フラグ
    bool texFlag;
}

//出力
struct Out
{
    //システム座標
    float4 svpos : SV_POSITION;
    //座標
    float4 pos : POSITION;
    //法線
    float3 normal : NORMAL;
    //uv
    float2 uv : TEXCOORD;
};

//入力
struct VSInput
{
    //座標
    float4 pos : POSITION;
    //法線
    float4 normal : NORMAL;
    //uv
    float2 uv : TEXCOORD;
};

//頂点シェーダ
Out VS(VSInput input)
{
    input.pos = mul(world, input.pos);
    input.pos = mul(view, input.pos);
    input.pos = mul(projection, input.pos);

    Out o;
    o.svpos  = input.pos;
    o.pos    = input.pos;
    o.uv     = input.uv;
    o.normal = mul(world, input.normal);

    return o;
}

//ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
    //光源ベクトル
    float3 light = normalize(float3(-1.0f, 1.0f, -1.0f));

    //内積
    float bright = dot(o.normal, light);

    //色
    float3 color = (texFlag == true ? tex.Sample(smp, o.uv).rgb : diffuse);

    return float4(color * bright, 1);
}