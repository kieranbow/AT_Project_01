struct Light
{
    float3 direction;
    float intensity;
    float4 ambient;
    float4 color;
};

struct LightResult
{
    float4 Diffuse;
    float4 Specular;
};

