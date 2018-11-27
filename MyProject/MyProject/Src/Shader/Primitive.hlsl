// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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

Texture2D<float> depth : register(t0);
SamplerState smp : register(s0);

// ���[���h�r���[�v���W�F�N�V����
cbuffer wvp : register(b0)
{
	//���[���h�s��
    float4x4 world;
	//�r���[�s��
    float4x4 view;
	//�v���W�F�N�V�����s��
    float4x4 projection;
    float4x4 light;
	//�E�B���h�E�T�C�Y
    float2 window;
}

// ����
struct Input
{
    float4 pos   : POSITION;
    //�@��
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

// �o��
struct Out
{
    float4 svpos : SV_POSITION;
    float4 pos   : POSITION;
   //�@��
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

// ���_�V�F�[�_
[RootSignature(RS)]
Out VS(Input input)
{
    //input.pos.xy = float2(-1.0f, 1.0f) + (input.pos.xy / float2((window.x / 2.0f), -(window.y / 2.0f)))
    Out o;
    //o.pos   = input.pos;

    o.pos     = mul(mul(light, world), input.pos);
    input.pos = mul(world, input.pos);
    input.pos = mul(view, input.pos);
    input.pos = mul(projection, input.pos);
    o.svpos = input.pos;
    //o.pos = input.pos;
    o.normal = input.normal;
    o.uv = input.uv;

    return o;
}

// �s�N�Z���V�F�[�_
float4 PS(Out o) : SV_TARGET
{
    //return depth.Sample(smp, o.uv);

    float2 pos = float2(normalize(o.pos.x), normalize(o.pos.y));
    float2 uv = (float2(1, 1) + o.uv * float2(1.0f, 1.0f)) * float2(1.0f, 0.5f);
    float dep = depth.Sample(smp,uv);

    return float4(dep,dep,dep, 1);

    float color = dep;

    if(o.pos.z > dep + 0.0005f)
    {
        color *= 0.7f;
    }

    return float4(color,color,color, 1);
}