// ���[�g�V�O�l�`���̒�`
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_VERTEX),"\
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
	//�E�B���h�E�T�C�Y
    float2 window;
}

// ����
struct Input
{
    float4 pos   : POSITION;
    float4 color : COLOR;
};

// �o��
struct Out
{
    float4 svpos : SV_POSITION;
    float4 pos   : POSITION;
    float4 color : COLOR;
};

// ���_�V�F�[�_
[RootSignature(RS)]
Out VS(Input input)
{
    input.pos.xy = float2(-1.0f, 1.0f) + (input.pos.xy / float2((window.x / 2.0f), -(window.y / 2.0f)));

    Out o;
    o.svpos = input.pos;
    o.pos   = input.pos;
    o.color = input.color;

    return o;
}

// �s�N�Z���V�F�[�_
float4 PS(Out o) : SV_TARGET
{
    return float4(o.color);
}