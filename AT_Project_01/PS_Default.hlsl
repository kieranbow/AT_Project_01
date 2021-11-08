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
    float4 diffuse;
};

struct LightResult
{
    float4 Diffuse;
    float4 Specular;
};

cbuffer frameBuffer : register(b0)
{
    Light light;
}

cbuffer MaterialProperties : register(b1)
{
    Material mat;
}

// Functions
float Diffuse(Light light, float3 l, float3 N)
{
    float NdotL = saturate(dot(light.direction, N));
    return light.diffuse * NdotL;
}

float4 Specular(Light light, float3 V, float3 L, float3 N)
{
    // Phong lighting
    float3 R = normalize(reflect(-L, N));
    float RdotV = max(0, dot(R, V));
    
    // Blinn-phong lighting
    float3 H = normalize(L + V);
    float NdotH = max(0, dot(N, H));
    
    return light.diffuse * pow(RdotV, mat.SpecularPower);
}

LightResult DirectionalLight(Light light, float3 v, float3 n)
{
    LightResult result;
    
    float3 l = light.direction.xyz;
    
    result.Diffuse = Diffuse(light, l, n);
    result.Specular = Specular(light, v, l, n);
    
    return result;
}

Texture2D frog : TEXTURE : register(t0);
SamplerState state : SAMPLER : register(s0);

// Main Entry Point for Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 tex = frog.Sample(state, input.texcoord);
    //input.normal = normalize(input.normal);
    //float4 finalColor = tex * light.ambient;
    //finalColor += saturate(dot(light.direction, input.normal) * light.diffuse * tex);
    //return finalColor;
    
    float3 L = -light.direction.xyz;
    float3 V = normalize(input.normal - input.worldPos);
    
    LightResult lit = DirectionalLight(light, V, input.normal);
    
    float4 emissive = mat.Emissive;
    float4 ambient = mat.ambient * light.ambient;
    float4 diffuse = mat.Diffuse * lit.Diffuse;
    float4 Specular = mat.Specular * lit.Diffuse;


    float4 finalColor = (emissive + ambient + diffuse + Specular) * tex;
    
    return finalColor;
}
