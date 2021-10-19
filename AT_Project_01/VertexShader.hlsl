// Main Entry Point for Vertex Shader
float4 main(float2 inPosition : POSITION) : SV_POSITION
{
    return float4(inPosition, 0, 1);
}