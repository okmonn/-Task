// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_VERTEX),"\
                    "DescriptorTable(CBV(b1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
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

//���[���h�r���[�v���W�F�N�V����
cbuffer wvp : register(b0)
{
	//���[���h�s��
    float4x4 world;
	//�r���[�s��
    float4x4 view;
    //�v���W�F�N�V�����s��
    float4x4 projection;
    float4x4 light;
    float4 eye;
     //�E�B���h�E�T�C�Y
    float2 window;
}

//�{�[��
cbuffer born : register(b1)
{
    //�{�[��
    matrix borns[256];
}

//�o��
struct Out
{
    //�V�X�e�����W
    float4 svpos : SV_POSITION;
    //���W
    float4 pos : POSITION;
    //�@��
    float3 normal : NORMAL;
    //�{�[��
    min16uint2 born : BORN;
     //�E�F�C�g
    min16uint weight : WEIGHT;
};

//����
struct VSInput
{
    //���W
    float4 pos : POSITION;
    //�@��
    float4 normal : NORMAL;
    //�{�[��
    min16uint2 born : BORN;
    //�E�F�C�g
    min16uint weight : WEIGHT;
};

//���_�V�F�[�_
[RootSignature(RS)]
Out VS(VSInput input)
{
    float w = input.weight / 100.0f;
    matrix m = borns[input.born.x] * w + borns[input.born.y] * (1.0f - w);
    m = mul(world, m);

    Out o;
    o.svpos = mul(mul(light, m), input.pos);
    m = mul(view, m);
    m = mul(projection, m);
    input.pos = mul(m, input.pos);
    //o.svpos = input.pos;
    o.pos = input.pos;
    o.normal = mul(world, input.normal);
    o.born = input.born;
    o.weight = input.weight;

    return o;
}

//�s�N�Z���V�F�[�_
[RootSignature(RS)]
float4 PS(Out o) : SV_TARGET
{
    return float4(1, 0, 0, 0);
}