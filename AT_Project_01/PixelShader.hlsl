struct PS_INPUT // Same as vertex shader
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

Texture2D frog : TEXTURE : register(t0);
SamplerState state : SAMPLER : register(s0);

// Main Entry Point for Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 tex = frog.Sample(state, input.texcoord);
    // float3 output = lerp(tex, input.color, 0.5);
    
    return tex;
}