struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
    VSOut vsout;
    vsout.worldPos = (float3) mul(float4(pos, 1.0f), model);
    vsout.normal = mul(n, (float3x3) model);
    vsout.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vsout;
}