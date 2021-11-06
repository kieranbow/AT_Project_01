struct VS_INPUT
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer constBuffer : register(b0)
{
    matrix transform;
};


// Main Entry Point for Vertex Shader
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), transform);
    output.color = input.color;
    output.texcoord = input.texcoord;
    
    return output;
}