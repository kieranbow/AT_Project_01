#pragma once
#include "Light.h"
#include "Material.h"

// https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0

struct WorldViewProj
{
	DirectX::XMMATRIX m_world; //16
	DirectX::XMMATRIX m_view; //16
	DirectX::XMMATRIX m_projection; //16
};

struct PerFrame
{
	Light light;
};

struct MaterialProperties
{
	Material mat;
};