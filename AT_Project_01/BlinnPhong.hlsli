#include <Light.hlsli>

float4 Diffuse(Light light, float3 L, float3 N)
{
    float NdotL = max(0, dot(N, L));
    return light.color * NdotL;
}

float4 Specular(Light light, float3 V, float3 L, float3 N, float specPow)
{
    // Phong lighting
    float3 R = normalize(reflect(-L, N));
    float RdotV = max(0, dot(R, V));
    
    // Blinn-phong lighting
    float3 H = normalize(L + V);
    float NdotH = max(0, dot(N, H));
    
    return light.color * pow(RdotV, specPow);
}

LightResult DirectionalLight(Light light, float3 V, float3 P, float3 N, float specPow)
{
    LightResult result;
    
    float3 L = -light.direction.xyz;
    
    result.Diffuse = Diffuse(light, L, N);
    result.Specular = Specular(light, V, L, N, specPow);
    
    return result;
}

LightResult ComputeLight(Light light, float3 eyePos, float3 P, float3 N, float specPow)
{
    float3 V = normalize(eyePos - P).xyz;
    
    LightResult finalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    LightResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    
    result = DirectionalLight(light, V, P, N, specPow);
    
    finalResult.Diffuse += result.Diffuse;
    finalResult.Specular += result.Specular;
    
    finalResult.Diffuse = saturate(finalResult.Diffuse);
    finalResult.Specular = saturate(result.Specular);
    
    return finalResult;
}

