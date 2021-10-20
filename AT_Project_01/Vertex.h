#pragma once
#include <DirectXMath.h>

struct Vertex
{
	// Struct Constructors
	Vertex() {}
	Vertex(float x, float y) : position(x, y) {}

	DirectX::XMFLOAT2 position;
};
