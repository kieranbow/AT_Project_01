#pragma once
#include <DirectXMath.h>

// Description
// Virtual class that any geometry class will inherit.
class Geometry
{
	public:
		Geometry(DirectX::XMVECTOR position, DirectX::XMVECTOR scale);
		virtual ~Geometry();

		virtual void Update();
		virtual void Draw();

	private:
};

