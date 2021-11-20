#pragma once
#include "Light.h"

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
