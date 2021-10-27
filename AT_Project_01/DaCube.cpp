#include "DaCube.h"
#include "ErrorChecker.h"

DaCube::DaCube(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	pVertexBuffer = std::make_unique<VertexBuffer>();
	pIndexBuffer = std::make_unique<IndexBuffer>();
	pVertConstBuffer = std::make_unique<ConstantBuffer>();
	pVertexShader = std::make_unique<VSShader>();
	pPixelShader = std::make_unique<PSShader>();

	// Vertex buffer
	vertices =
	{
		{-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f},
		{1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f},
		{-1.0f, 1.0f, -1.0f,	0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f},
		{-1.0f, -1.0f, 1.0f,	0.0f, 1.0f, 0.0f},
		{1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f},
	};
	pVertexBuffer->CreateVertexBuffer(device, vertices, 0u);
	pVertexBuffer->BindBuffer(0u, 1u, deviceContext);

	// Index buffer
	indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4,
	};
	pIndexBuffer->CreateIndexBuffer(device, indices);
	pIndexBuffer->BindBuffer(deviceContext, DXGI_FORMAT_R16_UINT, 0u);

	// Constant buffer
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};

	const ConstantBuffer cb =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(35) *
				DirectX::XMMatrixRotationX(20) *
				DirectX::XMMatrixTranslation(0, 0, 8.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f ,0.5f, 10.0f)
			)
		}
	};
	pVertConstBuffer->CreateConstantBuffer(device, cb);

	// Vertex Shader
	pVertexShader->ReadVSShaderToBlob(L"..\\x64\\Debug\\VertexShader.cso");
	pVertexShader->CreateVSShader(device);

	// Pixel Shader
	pPixelShader->ReadPSShaderToBlob(L"..\\x64\\Debug\\PixelShader.cso");
	pPixelShader->CreatePSShader(device);

}

void DaCube::Draw(ID3D11DeviceContext* deviceContext)
{
	pVertConstBuffer->SetVSConstBuffer(deviceContext, 0u, 1u);
	pVertexShader->SetVSShader(deviceContext, 0u);
	pPixelShader->SetPSShader(deviceContext, 0u);

	deviceContext->IASetInputLayout(pInputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
}
