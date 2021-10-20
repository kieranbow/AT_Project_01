struct PS_INPUT // Same as vertex shader
{
    float4 inPosition : SV_POSITION;
    float3 inColor : COLOR;
};

// Main Entry Point for Pixel Shader
float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(input.inColor, 1.0f);
}