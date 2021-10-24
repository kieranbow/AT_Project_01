#pragma once
#include <DirectXMath.h>

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

//struct Vertex
//{
	//Vertex() {}
	//Vertex(float x, float y, float z, float r, float g, float b)
		//: position(x, y, z), color(r, g, b) {}

	//Vertex(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT3 _color)
		//: position(_position.x, _position.y, _position.z), color(_color.x, _color.y, _color.z) {}

	//struct Position
	//{
		//Position() {}
		//Position(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
		//float x = 0;
		//float y = 0;
		//float z = 0;
	//} position;

	//struct Color
	//{
		//Color() {}
		//Color(float red, float green, float blue) : r(red), g(green), b(blue) {}
		//float r = 0;
		//float g = 0;
		//float b = 0;
	//} color;

//};