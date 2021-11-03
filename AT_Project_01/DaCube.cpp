#include "DaCube.h"
#include "ErrorChecker.h"
#include "Graphics.h"
#include <list>

DaCube::DaCube(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;

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
}

void DaCube::Update(float dt)
{
	rot += 0.05f;
	if (rot > 6.28f)
	{
		rot = 0.0f;
	}

	//transform.SetRotationAxis(rot);
	// transform.SetScale(1.0f, 1.0f, 1.0f);
	// transform.SetPosition(0.0f, 0.0f, rot);
	transform.Update();
}

void DaCube::Draw(Graphics* gfx)
{
	HRESULT hr;

	std::list<int> type;

	pVertConstBuffer->data.matrix = transform.GetWorldMatrix() * gfx->GetViewMatrix() * gfx->GetProjectionMatrix();

	// SetIAVertex buffer

	// pVertConstBuffer->data.matrix = world * gfx->GetViewMatrix() * gfx->GetProjectionMatrix();
	pVertConstBuffer->data.matrix = DirectX::XMMatrixTranspose(pVertConstBuffer->data.matrix);

	hr = pVertConstBuffer->UpdateBuffer(gfx->GetDeviceContext());
	Logging::ThrowIf(hr, "Constant buffer failed to update");
	pVertConstBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), 0u, 1u);

	pVertexShader->SetVSShader(gfx->GetDeviceContext(), 0u);
	pPixelShader->SetPSShader(gfx->GetDeviceContext(), 0u);

	gfx->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
}
