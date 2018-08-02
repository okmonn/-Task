//���[�g�V�O�l�`���̐錾
//#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),\
//CBV(b0),\
//SRV(t0),\
//DescriptorTable(CBV(b1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),\
//DescriptorTable(SRV(t1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),\
//StaticSampler(s0, filter = FILTER_MIN_MAG_MIP_LINEAR,\
//addressU = TEXTURE_ADDRESS_WRAP,\
//addressV = TEXTURE_ADDRESS_WRAP,\
//addressW = TEXTURE_ADDRESS_WRAP,\
//mipLodBias = 0.0f,\
//maxAnisotropy = 0,\
//comparisonFunc = COMPARISON_NEVER,\
//borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,\
//minLOD = 0.0f,\
//maxLOD = 3.402823466e+38f,\
//space = 0,\
//visibility = SHADER_VISIBILITY_ALL)"

#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),\
CBV(b0, space = 0, flags = DATA_STATIC),\
DescriptorTable(SRV(t0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),\
StaticSampler(s0, filter = FILTER_MIN_MAG_MIP_LINEAR,\
addressU = TEXTURE_ADDRESS_WRAP,\
addressV = TEXTURE_ADDRESS_WRAP,\
addressW = TEXTURE_ADDRESS_WRAP,\
mipLodBias = 0.0f,\
maxAnisotropy = 16,\
comparisonFunc = COMPARISON_NEVER,\
borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,\
minLOD = 0.0f,\
maxLOD = 3.402823466e+38f,\
space = 0,\
visibility = SHADER_VISIBILITY_ALL)"

//�e�N�X�`��
Texture2D<float4> tex : register(t0);
//�T���v���[
SamplerState smp : register(s0);

//���[���h�r���[�v���W�F�N�V����
cbuffer wvp : register(b0)
{
	//���[���h�s��
    float4x4 world;
	//�r���[�v���W�F�N�V�����s��
    float4x4 viewProjection;
}

//�E�B���h�E�T�C�Y�E�摜�T�C�Y
cbuffer size : register(b1)
{
	//�E�B���h�E�T�C�Y
	float2 window;
	//�摜�T�C�Y
	float2 imageSize;
	//�摜UV
	float2 imageUv;
}

//�o��
struct Out
{
	//�V�X�e�����W
    float4 svpos : SV_POSITION;
	//���W
    float4 pos : POSITION;
	//uv�l
    float2 uv : TEXCOORD;
};

//����
struct VSInput
{
	//���W
    float4 pos : POSITION;
	//uv
    float2 uv : TEXCOORD;
};

//���_�V�F�[�_
[RootSignature(RS)]
Out BasicVS(VSInput input)
{
	/*���W�␳
	pos.xy = float2(-1, 1) + (pos.xy / float2((640 / 2), -(480 / 2)));*/

    Out o;
    o.svpos = input.pos;
    o.pos = input.pos;
    o.uv = input.uv;
	
    return o;
}

//�s�N�Z���V�F�[�_
[RootSignature(RS)]
float4 BasicPS(Out o) : SV_TARGET
{
	//return float4(tex.Sample(smp, o.uv).rgb, 0);
    float4 t = tex.Sample(smp, o.uv);
    /*if (t.a <= 0.0)
    {
        discard;
    }
*/
	return float4(1, 1, 1, 1);
    return t;
}