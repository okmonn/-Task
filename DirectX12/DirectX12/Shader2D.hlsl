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
cbuffer image : register(b1)
{
	//�E�B���h�E�T�C�Y
	float2 window;
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
//[RootSignature(RS)]
Out BasicVS(VSInput input)
{
    //���W�␳
    //input.pos.xy = float2(-1, 1) + (input.pos.xy / float2((640 / 2.0f), -(480 / 2.0f)));
    //input.pos = mul(mul(viewProjection, world), input.pos);

    Out o;
    o.svpos = input.pos;
    o.pos = input.pos;
    o.uv = input.uv;
	
    return o;
}

//�s�N�Z���V�F�[�_
//[RootSignature(RS)]
float4 BasicPS(Out o) : SV_TARGET
{
	//return float4(tex.Sample(smp, o.uv).rgb, 0);
    float4 t = tex.Sample(smp, o.uv);
    if (t.a <= 0.0)
    {
        discard;
    }

    float bright = 2.5f;

    t.rgb *= float3(bright, bright, bright);

    return t;
}