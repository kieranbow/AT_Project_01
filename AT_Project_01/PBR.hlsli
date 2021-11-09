static float PI = 3.14159265359f;

float chiGGX(float v)
{
    return v > 0.0f ? 1.0f : 0.0f;
}

// Trowbridge-Reitz GGX normal distribution
float NormalDistributionGGX(float3 normalV, float3 halfV, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(normalV, halfV), 0.0f);
    float NdotH2 = NdotH * NdotH;
    
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
    
    return (chiGGX(NdotH2) * a2) / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    
    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;
    
    return nom / denom;
}

float GeometrySmith(float3 normal, float3 halfV, float3 light, float roughness)
{
    float NdotV = max(dot(normal, halfV), 0.0f);
    float NdotL = max(dot(normal, light), 0.0f);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

// Schlick's approximation
float3 FresnelSchlick(float cosTheta, float3 F0) // F0 = material response at normal incidence
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}
