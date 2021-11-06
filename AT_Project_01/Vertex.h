#pragma once
#include <DirectXMath.h>

// Description
// Vertex Struct that contrain a position and color.
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b)
		: position(x, y, z), normal(nx, ny, nz), vertColor(r, g, b) {}

	DirectX::XMFLOAT3 position;
	//DirectX::XMFLOAT2 texcoord;
	DirectX::XMFLOAT3 vertColor;
	DirectX::XMFLOAT3 normal;
};
