struct VS_INPUT
{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

cbuffer constBuffer
{
    matrix transform;
};


// Main Entry Point for Vertex Shader
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), transform);
    output.color = input.color;
    
    return output;
}