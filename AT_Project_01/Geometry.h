#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VSShader.h"
#include "PSShader.h"
#include "TransformComponent.h"

class Graphics;

// Description
// Virtual class that any geometry class will inherit.
class Geometry
{
	public:
		explicit Geometry(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {};
		virtual ~Geometry() = default;

		virtual void Create(float width, float height, float depth) = 0;
		virtual void Update(float dt) = 0;
		virtual void Draw(Graphics* gfx) = 0;
		
		TransformComponent transform;

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		std::unique_ptr<VertexBuffer> pVertexBuffer = std::make_unique<VertexBuffer>();
		std::unique_ptr<IndexBuffer>	pIndexBuffer = std::make_unique<IndexBuffer>();
		std::unique_ptr<ConstantBuffer<VertexConstBuffer>>	pVertConstBuffer = std::make_unique<ConstantBuffer<VertexConstBuffer>>();
		std::unique_ptr<VSShader> pVertexShader = std::make_unique<VSShader>();
		std::unique_ptr<PSShader> pPixelShader = std::make_unique<PSShader>();
};
