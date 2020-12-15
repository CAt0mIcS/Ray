cbuffer CBufLight
{
    float4 lightPos;
};

// Controls cube light color
static const float3 materialColor = { 0.7f, 0.7f, 0.9f };

// Controls ambient color and brightness
static const float3 ambient = { 0.05f, 0.05f, 0.05f };

// Sets the light color
static const float3 diffuseColor = { 0.0f, 0.5f, 0.5f };

// Controls how fast the light falls of
static const float diffuseIntensity = 1.0f;


static const float attConst = 0.3f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;


float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
    // fragment to light vector data
    float3 lightPos2 = (float3) lightPos;
    const float3 vToL = lightPos2 - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    // diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    
    // diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
    
    // final color
    return float4(saturate((diffuse + ambient) * materialColor), 1.0f);

}