#pragma once
#include <DirectXMath.h>

struct Vertex
{
	// Struct Constructors
	Vertex() {}
	Vertex(float x, float y, float z, float r, float g, float b) 
		: position(x, y, z), color(r,g,b) {}

	DirectX::XMFLOAT3 position = {0,0,0};
	DirectX::XMFLOAT3 color = {0,0,0};
};
