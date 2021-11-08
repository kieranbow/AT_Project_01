#pragma once
#include <DirectXMath.h>
struct Material
{
	DirectX::XMFLOAT4 Emissive;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	float SpecularPower;
	int UseTexture;
	float padding[2];
};

