#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

// A struct with propertices for lighting
struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	DirectX::XMFLOAT3 direction;	// Direction of light. Mainly for directional lighting | 12 bytes
	float intensity = 0;			// Unitless intensity of any light | 4 bytes
	DirectX::XMFLOAT4 ambientColor;	// Simple and fake GI for any light | 16 bytes
	DirectX::XMFLOAT4 color;		// Color of any light | 16 bytes.
};
