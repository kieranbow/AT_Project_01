#include "Cube.h"
#include "ErrorChecker.h"
#include "Graphics.h"

Cube::Cube(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;

	float width = 1.0f;
	float height = 1.0f;
	float depth = 1.0f;

	// Vertex buffer
	vertices =
	{
		//{-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f},
		//{1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f},
		//{-1.0f, 1.0f, -1.0f,	0.0f, 0.0f, 1.0f},
		//{1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f},
		//{-1.0f, -1.0f, 1.0f,	0.0f, 1.0f, 0.0f},
		//{1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f},
		//{-1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f},
		//{1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f},

		// Front
		{-width, -height, -depth, 1.0f, 0.0f, 0.0f},
		{-width, height, -depth, 0.0f, 1.0f, 0.0f},
		{width, height, -depth, 0.0f, 0.0f, 1.0f},
		{width, -height, -depth, 1.0f, 0.0f, 0.0f},

		// Back
		{-width, -height, depth, 1.0f, 0.0f, 0.0f},
		{width, -height, depth, 0.0f, 1.0f, 0.0f},
		{width, height, depth, 0.0f, 0.0f, 1.0f},
		{-width, height, depth, 1.0f, 0.0f, 0.0f},

		// Top
		{-width, height, -depth, 1.0f, 0.0f, 0.0f},
		{-width, height, depth, 0.0f, 1.0f, 0.0f},
		{width, height, depth, 0.0f, 0.0f, 1.0f},
		{width, height, -depth, 1.0f, 0.0f, 0.0f},

		// Bottom
		{-width, -height, -depth, 1.0f, 0.0f, 0.0f},
		{width, -height, -depth, 0.0f, 1.0f, 0.0f},
		{width, -height, depth, 0.0f, 0.0f, 1.0f},
		{-width, -height, depth, 1.0f, 0.0f, 0.0f},

		// left
		{-width, -height, depth, 1.0f, 0.0f, 0.0f},
		{-width, height, depth, 0.0f, 1.0f, 0.0f},
		{-width, height, -depth, 0.0f, 0.0f, 1.0f},
		{-width, -height, -depth, 1.0f, 0.0f, 0.0f},

		// Right
		{width, -height, -depth, 1.0f, 0.0f, 0.0f},
		{width, height, -depth, 0.0f, 1.0f, 0.0f},
		{width, height, depth, 0.0f, 0.0f, 1.0f},
		{width, -height, depth, 1.0f, 0.0f, 0.0f},
	};

	hr = pVertexBuffer->CreateVertexBuffer(device, vertices, 0u);
	Logging::ThrowIf(hr, "Vertex Failed to build");
	pVertexBuffer->BindBuffer(0u, 1u, deviceContext);

	// Index buffer
	indices =
	{
		//0,2,1, 2,3,1,
		//1,3,5, 3,7,5,
		//2,6,3, 3,6,7,
		//4,5,7, 4,7,6,
		//0,4,2, 2,4,6,
		//0,1,4, 1,5,4,

		0,1,2,0,2,3,
		4,5,6,4,6,7,
		8,9,10,8,10,11,
		12,13,14,12,14,15,
		16,17,18,16,18,19,
		20,21,22,20,22,23
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

void Cube::Update(float dt)
{
	rot += 0.05f;
	if (rot > 6.28f)
	{
		rot = 0.0f;
	}

	transform.SetRotationAxis(rot);
	// transform.SetScale(1.0f, 1.0f, 1.0f);
	// transform.SetPosition(0.0f, 0.0f, rot);
	transform.Update();
}

void Cube::Draw(Graphics* gfx)
{
	HRESULT hr;
	// SetIAVertex buffer

	// Assign constant buffer new data
	pVertConstBuffer->data.matrix = transform.GetWorldMatrix() * gfx->GetViewMatrix() * gfx->GetProjectionMatrix();
	pVertConstBuffer->data.matrix = DirectX::XMMatrixTranspose(pVertConstBuffer->data.matrix);

	// Update and set constant buffer
	hr = pVertConstBuffer->UpdateBuffer(gfx->GetDeviceContext());
	Logging::ThrowIf(hr, "Constant buffer failed to update");
	pVertConstBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), 0u, 1u);

	// Set Shaders
	pVertexShader->SetVSShader(gfx->GetDeviceContext(), 0u);
	pPixelShader->SetPSShader(gfx->GetDeviceContext(), 0u);

	// Draw
	gfx->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
}

void Cube::CreateSphere(float radius, int sliceCount, int stackCount)
{
	// http://www.richardssoftware.net/2013/07/shapes-demo-with-direct3d11-and-slimdx.html

	radius = 5.0f;
	sliceCount = 10;
	stackCount = 10;

	vertices =
	{
		{0, radius, 0, 1.0f, 0.0f, 1.0f},
	};

	auto phiStep = DirectX::XM_PI / stackCount;
	auto thetaStep = 2.0f * DirectX::XM_PI / sliceCount;


	for (int i = 0; i < stackCount - 1; i++)
	{
		auto phi = i * phiStep;

		for (int j = 0; j < sliceCount; j++)
		{
			auto theta = j * thetaStep;

			auto p = DirectX::XMFLOAT3(
				radius * sin(phi) * cos(theta),
				radius * cos(phi),
				radius * sin(phi) * sin(theta));

			auto t = DirectX::XMFLOAT3(
				-radius * sin(phi) * sin(theta),
				0,
				radius * sin(phi) * cos(theta));

			
		}
	}

}
