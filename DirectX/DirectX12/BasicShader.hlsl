Texture2D<float4> tex : register(t0);
SamplerState      smp : register(s0);

//ワールドビュープロジェクション
cbuffer wvp : register(b0)
{
	//ワールド行列
	float4x4 world;
	//ビュープロジェクション行列
	float4x4 viewProjection;
}

//画像データ
cbuffer image : register(b1)
{
    //ウィンドウサイズ
    float2 window;
}

//出力
struct Out
{
	//システム座標
	float4 svpos  : SV_POSITION;
	//座標
	float4 pos    : POSITION;
	//uv値
	float2 uv     : TEXCOORD;
};

//入力
struct Input
{
	//座標
	float4 pos : POSITION;
	//uv
	float2 uv  : TEXCOORD;
};

// 頂点シェーダ
Out VS(Input input)
{
    //テクスチャサイズの取得
    float2 size = float2(0.0f, 0.0f);
    tex.GetDimensions(size.x, size.y);

    input.pos.xy = float2(-1, 1) + (input.pos.xy / float2((window.x / 2), -(window.y / 2)));
    input.uv = input.uv / size;
    //input.pos = mul(mul(viewProjection, world), input.pos);
	Out o;
	o.svpos = input.pos;
	o.pos   = input.pos;
	o.uv    = input.uv;

	return o;
}

// ピクセルシェーダ
float4 PS(Out o) : SV_TARGET
{
    float4 ps = tex.Sample(smp, o.uv);
    if (ps.a <= 0.0)
    {
        discard;
    }
        
    return ps;
}
