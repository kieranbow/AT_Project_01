#pragma once
#include <DirectXMath.h>

struct Vertex
{
	// Struct Constructors
	Vertex() {}
	Vertex(float x, float y, float r, float g, float b) 
		: position(x, y), color(r,g,b) {}

	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT3 color;
};