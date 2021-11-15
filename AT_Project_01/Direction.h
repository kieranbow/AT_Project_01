#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Direction
{
	XMVECTOR v_forward = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR v_backward = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR v_left = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR v_right = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
};
