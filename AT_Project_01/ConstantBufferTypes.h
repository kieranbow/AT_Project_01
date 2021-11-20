#pragma once
#include "Light.h"
#include "Material.h"

struct WorldViewProj
{
	DirectX::XMMATRIX m_world; //16
	DirectX::XMMATRIX m_view; //16
	DirectX::XMMATRIX m_projection; //16
};

struct PerFrame
{
	Light light;
	DirectX::XMFLOAT4 eyePos;
};

struct MaterialProperties
{
	Material_BlinnPhong mat;
};