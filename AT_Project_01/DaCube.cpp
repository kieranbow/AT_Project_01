#include "DaCube.h"
#include "ErrorChecker.h"
#include "Graphics.h"

DaCube::DaCube(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;
	pVertexBuffer = std::make_unique<VertexBuffer>();
	pIndexBuffer = std::make_unique<IndexBuffer>();
	pVertConstBuffer = std::make_unique<ConstantBuffer>();
	pVertexShader = std::make_unique<VSShader>();
	pPixelShader = std::make_unique<PSShader>();
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

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

	hr = pVertexBuffer->CreateVertexBuffer(device, vertices, 0u);
	Logging::ThrowIf(hr, "Vertex Failed to build");
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
	hr = pIndexBuffer->CreateIndexBuffer(device, indices);
	Logging::ThrowIf(hr, "Index failed to build");
	pIndexBuffer->BindBuffer(deviceContext, DXGI_FORMAT_R16_UINT, 0u);

	// Constant buffer
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};

	DirectX::XMMATRIX xmm_world = DirectX::XMMatrixIdentity();
	static DirectX::XMVECTOR xmv_eyePos = DirectX::XMVectorSet(0.0f, -10.0f, -2.0f, 0.0f);
	static DirectX::XMVECTOR xmv_lookAtPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	static DirectX::XMVECTOR xmv_upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMFLOAT3 eyePosFloat3;
	DirectX::XMStoreFloat3(&eyePosFloat3, xmv_eyePos);
	eyePosFloat3.y += 0.05f;
	xmv_eyePos = DirectX::XMLoadFloat3(&eyePosFloat3);

	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(xmv_eyePos, xmv_lookAtPos, xmv_upVector);

	float fovDeg = 90.0f;
	float fovRad = (fovDeg / 360.0f) * DirectX::XM_2PI;
	float aspectRatio = 800.0f / 600.0f;
	float nearZ = 0.1f;
	float farZ = 1000.0f;

	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearZ, farZ);

	const ConstantBuffer cb =
	{
		{
			DirectX::XMMatrixTranspose(xmm_world* viewMatrix* projectionMatrix),
		}
	};
	hr = pVertConstBuffer->CreateConstantBuffer(device, cb);
	Logging::ThrowIf(hr, "const failed to build");

	// Vertex Shader
	pVertexShader->ReadVSShaderToBlob(L"..\\x64\\Debug\\VertexShader.cso");
	pVertexShader->CreateVSShader(device);

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	device->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pVertexShader->GetVSBlob()->GetBufferPointer(),
		pVertexShader->GetVSBlob()->GetBufferSize(),
		&pInputLayout);


	// Pixel Shader
	hr = pPixelShader->ReadPSShaderToBlob(L"..\\x64\\Debug\\PixelShader.cso");
	Logging::ThrowIf(hr, "Pixel failed to read");

	hr = pPixelShader->CreatePSShader(device);
	Logging::ThrowIf(hr, "pixel failed to create");


	deviceContext->IASetInputLayout(pInputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// configure viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1u, &vp);

}

void DaCube::Draw(ID3D11DeviceContext* deviceContext)
{
	pVertConstBuffer->SetVSConstBuffer(deviceContext, 0u, 1u);
	pVertexShader->SetVSShader(deviceContext, 0u);
	pPixelShader->SetPSShader(deviceContext, 0u);

	deviceContext->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
}
