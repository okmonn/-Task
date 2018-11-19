// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "StaticSampler(s0, "\
                                  "filter = FILTER_MIN_MAG_MIP_LINEAR, "\
                                  "addressU = TEXTURE_ADDRESS_WRAP, "\
                                  "addressV = TEXTURE_ADDRESS_WRAP, "\
                                  "addressW = TEXTURE_ADDRESS_WRAP, "\
                                  "mipLodBias = 0.0f, "\
                                  "maxAnisotropy = 0, "\
                                  "comparisonFunc = COMPARISON_NEVER, "\
                                  "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
                                  "minLOD = 0.0f, "\
                                  "maxLOD = 3.402823466e+38f, "\
                                  "space = 0, "\
                                  "visibility = SHADER_VISIBILITY_ALL)"

Texture2D<float4> tex : register(t0);
Texture2D<float> depth : register(t1);
SamplerState smp : register(s0);


// ����
struct Input
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD;
};

// �o��
struct Out
{
    float4 svpos : SV_POSITION;
    float4 pos   : POSITION;
    float2 uv    : TEXCOORD;
};

// ���_�V�F�[�_
[RootSignature(RS)]
Out VS(Input input)
{
    Out o;
    o.svpos = input.pos;
    o.pos   = input.pos;
    o.uv    = input.uv;

    return o;
}

// ����
float Mono(Out o)
{
    return dot(float3(0.298912f, 0.586611f, 0.114478f), tex.Sample(smp, o.uv).rgb);
}

// ���]
float4 Reverse(Out o)
{
    return float4(1.0f - tex.Sample(smp, o.uv).rgb, tex.Sample(smp, o.uv).a);

}

// �s�N�Z���V�F�[�_
float4 PS(Out o) : SV_TARGET
{
    float tmp = pow(depth.Sample(smp, o.uv), 10);
    if(o.uv.x < 0.5f)
    {
        return tmp;
    }
    

    //�摜�T�C�Y
    float2 size = float2(0.0f, 0.0f);
    tex.GetDimensions(size.x, size.y);

    //�אڂ���s�N�Z��
    float2 adjacent = float2(1.0f / size.x, 1.0f / size.y);

    float4 ret = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //��̃s�N�Z��
    ret += tex.Sample(smp, o.uv + float2(-2.0f * adjacent.x, 2.0f * adjacent.y))  / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 2.0f * adjacent.x, 2.0f * adjacent.y))  / 9.0f;
    ret += tex.Sample(smp, o.uv + float2(                 0, 2.0f * adjacent.y))  / 9.0f;
 
    //�^�񒆂̃s�N�Z��
    ret += tex.Sample(smp, o.uv + float2(-2.0f * adjacent.x, 0.0f))               / 9.0f;
    ret += tex.Sample(smp, o.uv)                                                  / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 2.0f * adjacent.x, 0.0f))               / 9.0f;
 
    //���̃s�N�Z��
    ret += tex.Sample(smp, o.uv + float2(-2.0f * adjacent.x, -2.0f * adjacent.y)) / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 2.0f * adjacent.x, -2.0f * adjacent.y)) / 9.0f;
    ret += tex.Sample(smp, o.uv + float2( 0.0f             , -2.0f * adjacent.y)) / 9.0f;

    //ret = ret * 2 - tex.Sample(smp, o.uv + float2(-adjacent.x, 0)) - tex.Sample(smp, o.uv + float2(adjacent.x, 0));

    //float color = dot(float3(0.298912f, 0.586611f, 0.114478f), ret.rgb);

    return float4(ret.r, ret.g, ret.b, 1);

}