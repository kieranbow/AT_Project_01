#pragma once
#include <DirectXMath.h>

// Description
// Vertex Struct that contrain a position and color.
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float u, float v)
		: position(x, y, z), normal(nx, ny, nz), vertColor(r, g, b), texcoord(u, v) {}

	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 normal = { 0,0,0 };
	DirectX::XMFLOAT3 vertColor = { 0,0,0 };
	DirectX::XMFLOAT2 texcoord = { 0,0 };
};
