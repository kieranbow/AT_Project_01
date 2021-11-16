#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	DirectX::XMFLOAT3 direction; // 12
	float intensity = 0; // 4
	DirectX::XMFLOAT4 ambientColor; // 16
	DirectX::XMFLOAT4 color; // 16
};
