#include <BlinnPhong.hlsli>

struct PS_INPUT // Same as vertex shader
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

struct Material
{
    float4 Emissive;
    float4 ambient;
    float4 Diffuse;
    float4 Specular;
    float SpecularPower;
    bool UseTexture;
    float2 padding;
};

cbuffer frameBuffer : register(b0)
{
    Light light;
    float4 eyePos;
}

cbuffer MaterialProperties : register(b1)
{
    Material mat;
}

Texture2D albedoMap : TEXTURE : register(t0);
Texture2D normalMap : TEXTURE : register(t1);
SamplerState state : SAMPLER : register(s0);

// Main Entry Point for Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    
    float4 albedo = albedoMap.Sample(state, input.texcoord);
    float3 normal = input.normal * normalMap.Sample(state, input.texcoord).rgb;
    normal = normalize(normal);

    LightResult lit = ComputeLight(light, eyePos.xyz, input.worldPos, normal, mat.SpecularPower);
    
    float4 emissive = mat.Emissive;
    float4 ambient = mat.ambient * light.ambient;
    float4 diffuse = mat.Diffuse * lit.Diffuse;
    float4 Specular = mat.Specular * lit.Specular;
    
    float4 finalColor = (emissive + ambient + diffuse + Specular) * albedo;
    return finalColor;
}
