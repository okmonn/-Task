// ���[�g�V�O�l�`���̒�`
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

//�}�e���A��
cbuffer mat : register(b1)
{
    //��{�F
    float3 diffuse;
    //�����x
    float alpha;
    //���ˋ��x
    float specularity;
	//���ːF
    float3 specula;
    //���F
    float3 mirror;
    //��Z�e�N�X�`���Ή��t���O
    bool sphFlag;
    //���Z�e�N�X�`���Ή��t���O
    bool spaFlag;
    //�e�N�X�`���Ή��t���O
    bool texFlag;
}

//�{�[��
cbuffer born : register(b2)
{
    //�{�[��
    matrix borns[256];
}

//�o��
struct Out
{
    //�V�X�e�����W
    float4 svpos     : SV_POSITION;
    //���W
    float4 pos       : POSITION;
    //�@��
    float3 normal    : NORMAL;
    //uv
    float2 uv        : TEXCOORD;
    //�{�[��
    min16uint2 born  : BORN;
     //�E�F�C�g
    min16uint weight : WEIGHT;
};

//����
struct VSInput
{
    //���W
    float4 pos       : POSITION;
    //�@��
    float4 normal    : NORMAL;
    //uv
    float2 uv        : TEXCOORD;
    //�{�[��
    min16uint2 born  : BORN;
    //�E�F�C�g
    min16uint weight : WEIGHT;
};

//���_�V�F�[�_
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

//�s�N�Z���V�F�[�_
[RootSignature(RS)]
float4 PS(Out o) : SV_TARGET
{
    //���C�g�̐F
    float3 lightCol = float3(0.6f, 0.6f, 0.6f);

    //�����x�N�g��
    float3 eye = float3(0.0f, 10.0f, -15.0f);
    float3 ray = normalize(o.pos.xyz - eye);

    //�����x�N�g��
    float3 light = normalize(float3(-1.0f, 1.0f, -1.0f));

    //���˃x�N�g��
    float3 ref = reflect(-light, o.normal);

    //�X�؃L����
    float spec = pow(saturate(dot(ref, ray)), specularity);

    //�����x�N�g���Ɩ@���Ƃ̓���
    float bright = saturate(dot(light, o.normal));

    //�����o�[�g
    bright = saturate(acos(bright) / 3.14159265f);

    float4 toon = ton.Sample(smp, float2(0.0f, 1.0f - bright));

    //�F
    float3 color = (texFlag == true ? tex.Sample(smp, o.uv).rgb : saturate(diffuse * bright + specula * spec + mirror * lightCol));
    color = (sphFlag == true ? color * sph.Sample(smp, o.uv).rgb : color);
    color = (spaFlag == true ? color + spa.Sample(smp, o.uv).rgb : color);

    return float4(color, alpha);
}