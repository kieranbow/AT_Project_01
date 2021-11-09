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
    input.normal = normalize(input.normal);
    
    float4 albedo = albedoMap.Sample(state, input.texcoord);
    float3 normal = input.normal * normalMap.Sample(state, input.texcoord).rgb;
    normal = normalize(normal);

    LightResult lit = ComputeLight(input.worldPos, normal);
    
    float4 emissive = mat.Emissive;
    float4 ambient = mat.ambient * light.ambient;
    float4 diffuse = mat.Diffuse * lit.Diffuse;
    float4 Specular = mat.Specular * lit.Specular;


    float4 finalColor = (emissive + ambient + diffuse + Specular) * albedo;
    
    return finalColor;
}
