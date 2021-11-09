#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	DirectX::XMFLOAT3 direction;
	float padding = 0;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 color;
};
