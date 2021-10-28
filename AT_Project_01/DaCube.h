#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VSShader.h"
#include "PSShader.h"

class Graphics;

class DaCube
{
	public:
		// DaCube(std::unique_ptr<Graphics>& gfx);
		DaCube(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		~DaCube() = default;

		DaCube(const DaCube&) = delete;
		DaCube& operator=(const DaCube&) = delete;

		void Draw(ID3D11DeviceContext* deviceContext);


	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		std::unique_ptr<VertexBuffer>		pVertexBuffer;
		std::unique_ptr<IndexBuffer>			pIndexBuffer;
		std::unique_ptr<ConstantBuffer>	pVertConstBuffer;

		std::unique_ptr<VSShader> pVertexShader;
		std::unique_ptr<PSShader> pPixelShader;
};

