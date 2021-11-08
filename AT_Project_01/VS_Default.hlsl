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
    float3 worldPos : WORLD_POSITION;
};

cbuffer perObject : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

// Main Entry Point for Vertex Shader
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    input.position.w = 1.0f;
    
    // World * View * Projection
    output.position = mul(world, input.position);
    output.position = mul(view, output.position);
    output.position = mul(projection, output.position);

    // Pass UVs
    output.texcoord = input.texcoord;
    
    // Calculate and normalize mesh normals for pixel shader
    output.normal = normalize(mul(float4(input.normal, 0.0f), world));

    
    output.worldPos = mul(world, input.position);
    return output;
}