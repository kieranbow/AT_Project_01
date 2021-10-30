#include "DaCube.h"
#include "ErrorChecker.h"
#include "Graphics.h"

DaCube::DaCube(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;
	pVertexBuffer = std::make_unique<VertexBuffer>();
	pIndexBuffer = std::make_unique<IndexBuffer>();
	pVertConstBuffer = std::make_unique<ConstantBuffer<VertexConstBuffer>>();
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

	// Constant Buffer
	hr = pVertConstBuffer->CreateConstantBuffer(device);
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

void DaCube::Update(float dt)
{
	rot += 0.05f;
	if (rot > 6.28f)
	{
		rot = 0.0f;
	}
	world = DirectX::XMMatrixIdentity();

	m_scale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, 0.0f);
	m_rotation = DirectX::XMMatrixRotationAxis(rotaxis, rot);
	m_translation = DirectX::XMMatrixTranslation(position.x, position.y, position.x);

	world =  m_scale * m_rotation * m_translation;
}

void DaCube::SetScale(float x, float y, float z)
{
	scale = { x, y, z };
}

void DaCube::SetPosition(float x, float y, float z)
{
	position = {x, y, z};
}

void DaCube::SetRotation(float x, float y, float z)
{
	rotation = { x, y, z };
}

void DaCube::Draw(ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;

	static DirectX::XMVECTOR eyePos = DirectX::XMVectorSet(0.0f, -4.0f, -2.0f, 0.0f);
	static DirectX::XMVECTOR lookAtPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	static DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eyePos, lookAtPos, upVector);

	float fovDegrees = 90.0f;
	float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
	float aspectRatio = 800.0f / 600.0f;
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);

	pVertConstBuffer->data.matrix = world * view * projection;
	pVertConstBuffer->data.matrix = DirectX::XMMatrixTranspose(pVertConstBuffer->data.matrix);

	hr = pVertConstBuffer->UpdateBuffer(deviceContext);
	Logging::ThrowIf(hr, "Constant buffer failed to update");
	pVertConstBuffer->SetVSConstBuffer(deviceContext, 0u, 1u);

	pVertexShader->SetVSShader(deviceContext, 0u);
	pPixelShader->SetPSShader(deviceContext, 0u);

	deviceContext->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
}
