struct PS_INPUT // Same as vertex shader
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

// Main Entry Point for Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}