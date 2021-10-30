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

		void Update(float dt);

		void SetScale(float x, float y, float z);
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);

		void Draw(ID3D11DeviceContext* deviceContext);

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		std::unique_ptr<VertexBuffer>		pVertexBuffer;
		std::unique_ptr<IndexBuffer>			pIndexBuffer;
		std::unique_ptr<ConstantBuffer<VertexConstBuffer>>	pVertConstBuffer;

		std::unique_ptr<VSShader> pVertexShader;
		std::unique_ptr<PSShader> pPixelShader;

		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

		DirectX::XMFLOAT3 scale = {1.0f, 1.0f, 1.0f};
		DirectX::XMFLOAT3 rotation = { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

		DirectX::XMMATRIX m_rotation;
		DirectX::XMMATRIX m_scale;
		DirectX::XMMATRIX m_translation;
		float rot = 0.01f;
};

