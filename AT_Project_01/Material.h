#pragma once
#include <DirectXMath.h>

// BlinnPhong material struct with propertices like color and specular.
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

// PBR material struct with propertices like baseColor, metallic and roughness.
struct Material_PBR
{
	DirectX::XMFLOAT4 baseColor; // 16 bytes
	float metallic; // Intensity of metallic | 4 bytes
	float roughness; // Intensity of roughness | 4 bytes
	float ambientOcculsion; // Intensity of ambient Occulsion | 4 bytes
	float padding; // 4 bytes
};
