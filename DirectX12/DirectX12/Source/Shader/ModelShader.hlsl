Texture2D<float4> tex : register(t0);
Texture2D<float4> spa : register(t1);
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
    //透明度
    float alpha;
    //反射強度
    float specularity;
	//反射色
    float3 specula;
    //環境色
    float3 mirror;
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
    float4 svpos  : SV_POSITION;
    //座標
    float4 pos    : POSITION;
    //法線
    float3 normal : NORMAL;
    //uv
    float2 uv     : TEXCOORD;
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
Out VS(VSInput input)
{
    float w1 = (float) (input.weight) / 100.0f;
    float w2 = (float) (1.0f - w1);
    matrix m = borns[input.born[0]] * w1 + borns[input.born[1]] * w2;
    m = mul(world, m);
    m = mul(view, m);
    m = mul(projection, m);

    //input.pos = mul(world, input.pos);
    //input.pos = mul(view, input.pos);
    //input.pos = mul(projection, input.pos);
    input.pos = mul(m, input.pos);

    Out o;
    o.svpos  = input.pos;
    o.pos    = input.pos;
    o.uv     = input.uv;
    o.normal = mul(world, input.normal);

    /*
    if(input.born[0] == 19 || input.born[1] == 19)
    {
        o.color = float4(1.0f, 0.0f, 1.0f) * (float(input.weight) / 100.0f);
    */

    return o;
}

//ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
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

    //色
    float3 color = (texFlag == true ? tex.Sample(smp, o.uv).rgb : saturate(diffuse * bright + specula * spec + mirror));

    return float4(color, alpha);
}