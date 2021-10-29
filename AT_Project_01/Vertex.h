#pragma once
#include <DirectXMath.h>

// Description
// Vertex Struct that contrain a position and color.
struct Vertex
{
	struct
	{
		float x	= 0;
		float y	= 0;
		float z	= 0;
	} position;

	struct 
	{
		float r	= 0;
		float g	= 0;
		float b	= 0;
	} color;
};
