// ルートシグネチャの宣言
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(CBV(b1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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

// 波形データ
float wave[8192] : register(b0);

// データサイズ
uint size : register(b1);

// 実部
RWStructuredBuffer<float> real : register(u0);
// 虚部
RWStructuredBuffer<float> imag : register(u1);

// 円周率
#define PI 3.14159265f

// ハニング窓関数
float Hanning(uint i)
{
    return (size % 2 == 0) ? 
    //偶数
    0.5f - 0.5f * cos(2.0f * PI * i / size) : 
    //奇数
    0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / size);
}

[numthreads(size, size, 1)]
void CS( uint3 id : SV_DispatchThreadID )
{
    float hanning = Hanning(id.x);
    float tmp = wave[id.y] * hanning;

    float r =  cos(2.0f * PI * id.x * id.y / size);
    float i = -sin(2.0f * PI * id.x * id.y / size);

    real[id.x] += r * tmp - i * 0.0f;
    imag[id.x] += r * 0.0f + i * tmp;
}