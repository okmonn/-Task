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

//出力
struct Out
{
	//システム座標
    float4 svpos : SV_POSITION;
	//座標
    float4 pos : POSITION;
    //色
    float4 color : COLOR;
};

//入力
struct Input
{
	//座標
    float4 pos : POSITION;
    //色
    float4 color : COLOR;
};

// 頂点シェーダ
Out VS(Input input)
{
    input.pos.xy = float2(-1.0f, 1.0f) + (input.pos.xy / float2((window.x / 2.0f), -(window.y / 2.0f)));

    //input.pos = mul(world, input.pos);
    //input.pos = mul(view, input.pos);
    //input.pos = mul(projection, input.pos);

    Out o;
    o.svpos = input.pos;
    o.pos   = input.pos;
    o.color = input.color;

    return o;
}

// ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
    if(o.color.a <= 0.0f)
    {
        discard;
    }

    return o.color;
}
