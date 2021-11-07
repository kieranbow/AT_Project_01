struct PS_INPUT // Same as vertex shader
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPosition : WORLD_POSITION;
};

cbuffer lightBuffer : register(b0)
{
    float3 ambientlightColor;
    float ambientlightStength;
    
    float3 dynamicLightColor;
    float dynamicLightStrenghth;
    float dynamicLightPosition;
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
    
    
    float3 ambientLight = ambientlightColor * ambientlightStength;
    
    float3 lightVector = normalize(dynamicLightPosition - input.worldPosition);
    
    float3 NdotL = max(dot(lightVector, input.normal), 0);
    
    float3 diffuseLighting = NdotL * dynamicLightStrenghth * dynamicLightColor;
    
    ambientLight += diffuseLighting;
    
    float3 final = tex.rgb * saturate(ambientLight);
    
    
    return float4(final, 1.0f);
}