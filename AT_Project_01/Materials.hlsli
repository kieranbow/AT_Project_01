// HLSL header file for materials used by both BlinnPhong and PBR shaders

struct BlinnPhong_material
{
    float4 Emissive;
    float4 ambient;
    float4 Diffuse;
    float4 Specular;
    float SpecularPower;
    bool UseTexture;
    float2 padding;
};

struct PBR_material
{
    float4 baseColor;
    float metallic;
    float roughness;
    float ambientOcculsion;
};

