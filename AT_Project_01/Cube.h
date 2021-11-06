#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VSShader.h"
#include "PSShader.h"

#include "TransformComponent.h"

class Graphics;

class Cube
{
	public:
		Cube(Graphics* gfx);
		~Cube() = default;

		Cube(const Cube&) = delete;
		Cube& operator=(const Cube&) = delete;

		void CreateCube();
		void CreateSphere(float radius, int sliceCount, int stackCount);
		void CreateCylinder();
		void CreatePyramid();
		void CreatePlane();

		void Update(float dt);
		void Draw(Graphics* gfx);

		TransformComponent transform;

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		std::unique_ptr<VertexBuffer> pVertexBuffer = std::make_unique<VertexBuffer>();
		std::unique_ptr<IndexBuffer>	pIndexBuffer = std::make_unique<IndexBuffer>();
		std::unique_ptr<ConstantBuffer<VertexConstBuffer>>	pWVPconstBuffer = std::make_unique<ConstantBuffer<VertexConstBuffer>>();
		std::unique_ptr<ConstantBuffer<PerObject>>	pPerObjectBuffer = std::make_unique<ConstantBuffer<PerObject>>();
		std::unique_ptr<VSShader> pVertexShader = std::make_unique<VSShader>();
		std::unique_ptr<PSShader> pPixelShader = std::make_unique<PSShader>();

		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

		float rot = 0.01f;
};
