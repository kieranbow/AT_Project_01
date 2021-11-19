#include <PBR.hlsli>

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
//Texture2D normalMap : TEXTURE : register(t1);
Texture2D RMAO : TEXTURE : register(t1);
TextureCube skyIR : register(t3);
TextureCube skyPrefilter : register(t4);
Texture2D BDRFlut : register(t5);

SamplerState samplerState : SAMPLER : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    // Base propertices
    float4 albedo = albedoMap.Sample(samplerState, input.texcoord);
    float3 normal = normalize(input.normal);
    float roughness = RMAO.Sample(samplerState, input.texcoord).r;
    float metallic = RMAO.Sample(samplerState, input.texcoord).g;
    float ao = RMAO.Sample(samplerState, input.texcoord).b;
    float specularIntensity = 0.5f;
    float ior = albedo.r + 1.0f; //0.04f

    //roughness = roughness * 0.0f;
    //metallic = metallic * 1.0f;
    
    float3 viewDir = normalize(eyePos.xyz - input.worldPos);
    float3 reflecVector = reflect(-viewDir, normal);
    
    float3 lightPos = float3(0.0f, 0.0f, 0.0f);
    
    // Calculate color at normal incidence
    float3 F0 = float3(0.04f, 0.04f, 0.04f); //abs((1.0f - ior) / (1.0 + ior));
    F0 = lerp(F0, albedo.rgb, metallic);

    float3 lo = float3(0.0f, 0.0f, 0.0f);

    // Calculate based on num of light. Currently only directional light is supported.
    for (int i = 0; i < 1; ++i)
    {
        float3 lightDir = normalize(-light.direction);
        float3 halfV = normalize(viewDir + lightDir);
        float distance = length(lightPos - input.worldPos);
        float attenuation = 1.0f / (distance * distance);
        float3 radiance = light.color.rgb * attenuation;
        
        // Cook-torrance BRDF
        float D = NormalDistributionGGX(normal, halfV, roughness);
        float G = GeometrySmith(normal, viewDir, lightDir, roughness);
        float3 F = FresnelSchlick(max(dot(halfV, viewDir), 0.0f), F0);
        
        // Calculate specular contribution
        float3 KS = F;
        float3 KD = float3(1.0f, 1.0f, 1.0f) - KS;
        KD *= 1.0f - metallic;
        
        float3 numerator = D * G * F;
        float denominator = 4.0f * max(dot(normal, viewDir), 0.0f) * max(dot(normal, lightDir), 0.0f);
        float3 specular = numerator / max(denominator, 0.001f);

        // Out going radiance
        float NdotL = max(dot(normal, lightDir), 0.0f);
        lo += (KD * albedo.rgb / PI + specular) * radiance * NdotL;
    }

    float3 KS = FresnelSchlickRoughness(max(dot(normal, viewDir), 0.0f), F0, roughness);
    float3 KD = float3(1.0f, 1.0f, 1.0f) - KS;
    KD *= 1.0f - metallic;
    
    // Sample irradiance texture of the sky and apply it to albedo
    float3 irradiance = skyIR.Sample(samplerState, normal).rgb;
    float3 diffuse = irradiance * albedo.rgb;

    // Use the cubemap and brdf lookup table and apply them together to create specular
    float3 preFilteredColor = skyPrefilter.Sample(samplerState, reflecVector).rgb;
    float2 brdf = BDRFlut.Sample(samplerState, float2(max(dot(normal, viewDir), 0.0f), roughness)).rg;
    float3 specular = preFilteredColor * (KS * brdf.x + brdf.y) * specularIntensity;

    float3 ambient = (KD * (diffuse * light.color.rgb) + specular) * ao;
    float3 color = ambient + lo;

    // Gamma correction
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

    return float4(color, 1.0f);
}
