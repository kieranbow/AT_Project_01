#pragma once
#include <DirectXMath.h>

class Geometry
{
	public:
		Geometry(DirectX::XMVECTOR position, DirectX::XMVECTOR scale);
		virtual ~Geometry();

		virtual void Update();
		virtual void Draw();

	private:
};

