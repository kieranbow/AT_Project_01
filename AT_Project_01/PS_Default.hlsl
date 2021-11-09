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

// Functions
float Diffuse(Light light, float3 L, float3 N)
{
    float NdotL = max(0, dot(N, L));
    return light.color * NdotL;
}

float4 Specular(Light light, float3 V, float3 L, float3 N)
{
    // Phong lighting
    float3 R = normalize(reflect(-L, N));
    float RdotV = max(0, dot(R, V));
    
    // Blinn-phong lighting
    float3 H = normalize(L + V);
    float NdotH = max(0, dot(N, H));
    
    return light.color * pow(RdotV, mat.SpecularPower);
}

LightResult DirectionalLight(Light light, float3 V, float3 P, float3 N)
{
    LightResult result;
    
    float3 L = -light.direction.xyz;
    
    result.Diffuse = Diffuse(light, L, N);
    result.Specular = Specular(light, V, L, N);
    
    return result;
}

LightResult ComputeLight(float3 P, float3 N)
{
    float3 V = normalize(eyePos.xyz - P).xyz;
    
    LightResult finalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    LightResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    
    result = DirectionalLight(light, V, P, N);
    
    finalResult.Diffuse += result.Diffuse;
    finalResult.Specular += result.Specular;
    
    finalResult.Diffuse = saturate(finalResult.Diffuse);
    finalResult.Specular = saturate(result.Specular);
    
    return finalResult;
}


Texture2D albedoMap : TEXTURE : register(t0);
Texture2D normalMap : TEXTURE : register(t1);
SamplerState state : SAMPLER : register(s0);

// Main Entry Point for Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
    //input.normal = normalize(input.normal);
    
    //float4 albedo = albedoMap.Sample(state, input.texcoord);
    //float3 normal = input.normal * normalMap.Sample(state, input.texcoord).rgb;
    //normal = normalize(normal);

    //LightResult lit = ComputeLight(input.worldPos, normal);
    
    //float4 emissive = mat.Emissive;
    //float4 ambient = mat.ambient * light.ambient;
    //float4 diffuse = mat.Diffuse * lit.Diffuse;
    //float4 Specular = mat.Specular * lit.Specular;
    
    //float4 finalColor = (emissive + ambient + diffuse + Specular) * albedo;
    //return finalColor;
    
    float4 albedo = albedoMap.Sample(state, input.texcoord);
    input.normal = normalize(input.normal);
    
    float ior = albedo.r + 1.0f; //0.04f
    float metallic = 0.0f;
    float roughness = 0.5f;
    float3 specular = 0.0f;
    float3 normalV = input.normal;
    

    float3 viewDir = normalize(eyePos.xyz - input.worldPos);
    float3 reflecVector = reflect(-viewDir, normalV);
    
    // Calculate color at normal incidence
    
    float3 F0 = abs((1.0f - ior) / (1.0 + ior));
    F0 = F0 * F0;
    F0 = lerp(F0, albedo.rgb, metallic);
    
    float3 rad = float3(0.0f, 0.0f, 0.0f);
    float3 lo = float3(0.0f, 0.0f, 0.0f);
    float3 lightPos = float3(0.0f, 0.0f, 0.0f);    
    
    for (int i = 0; i < 2; i++)
    {
        float3 lightDir = -light.direction;
        float3 halfV = normalize(viewDir + lightDir);
        float distance = length(lightPos - input.worldPos);
        float attenuation = 1.0f / (distance * distance);
        float3 radiance = light.color.rgb * attenuation;
        
        // Cook-torrance BRDF
        float D = NormalDistributionGGX(normalV, halfV, roughness);
        float G = GeometrySmith(normalV, viewDir, lightDir, roughness);
        float3 F = FresnelSchlick(max(dot(halfV, viewDir), 0.0f), F0);
        
        // Calculate specular contribution
        float3 KS = F;
        float3 KD = float3(1.0f, 1.0f, 1.0f) - KS;
        KD *= 1.0f - metallic;
        
        float3 numerator = D * G * F;
        float denominator = 4.0f * max(dot(normalV, viewDir), 0.0f) * max(dot(normalV, viewDir), 0.0f);
        specular = numerator / max(denominator, 0.001f);

        // Out going radiance
        float NdotL = max(dot(normalV, lightDir), 0.0f);
        rad = (((KD * albedo.rgb / PI) + specular) * radiance * NdotL);
        lo += rad;
    }
    
    float3 KS = FresnelSchlickRoughness(max(dot(normalV, viewDir), 0.0f), F0, roughness);
    float3 KD = 1.0f - KS;
    KD *= 1.0f - metallic;
    
    //float3 irradiance = float3(0.2f, 0.2f, 0.2f);
    //float3 diffuse = albedo.rgb * irradiance;


    float3 ambient = KD * albedo.rgb + specular;
    float3 color = ambient + lo;

    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

    return float4(color, 1.0f);
}
