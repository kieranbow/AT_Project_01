#pragma once
#include <DirectXMath.h>
struct Material_BlinnPhong
{
	DirectX::XMFLOAT4 Emissive;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	float SpecularPower;
	int UseTexture;
	float padding[2];
};

struct Material_PBR
{
	DirectX::XMFLOAT4 baseColor; // 16
	float metallic_strength; // 4
	float roughness_strength; // 4
	float ao_strength; // 4
	float padding; // 4
};

