struct PS_INPUT // Same as vertex shader
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct Light
{
    float3 direction;
    float4 ambient;
    float4 diffuse;
};

cbuffer frameBuffer : register(b0)
{
    Light light;
}

Texture2D frog : TEXTURE : register(t0);
SamplerState state : SAMPLER : register(s0);

// Main Entry Point for Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 tex = frog.Sample(state, input.texcoord);
    
    //float mask = tex.r + tex.g + tex.b;
    
    //float4 blend = float4(input.normal.r, input.normal.g, input.normal.b, 1.0f);
    //float4 output = lerp(float4(0.0f, 0.0f, 0.0f, 1.0f), blend, mask);
    //float4 output = float4(input.normal, 1.0f);

    input.normal = normalize(input.normal);
    float4 finalColor = tex * light.ambient;
    finalColor += saturate(dot(light.direction, input.normal) * light.diffuse * tex);
    
    return finalColor;
    
    //return float4(final, 1.0f);
}