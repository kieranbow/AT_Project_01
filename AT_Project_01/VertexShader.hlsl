struct VS_INPUT
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer perObject : register(b0)
{
    matrix world;
};

cbuffer perFrame : register(b1)
{
    matrix view;
    matrix projection;
}


// Main Entry Point for Vertex Shader
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    //output.position = mul(float4(input.position, 1.0f), transform);
    
    input.position.w = 1.0f;
    
    output.position = mul(world, input.position);
    output.position = mul(view, output.position);
    output.position = mul(projection, output.position);

    output.normal = normalize(input.normal);
    
    output.texcoord = input.texcoord;
    
    return output;
}