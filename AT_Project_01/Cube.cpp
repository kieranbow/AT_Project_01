#include "Cube.h"
#include "ErrorChecker.h"
#include "Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Cube::Cube(Graphics* gfx)
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
		Vertex(-width, -height, -depth,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f, 0.0f,	2.0f / 3.0f, 0.0f / 4.0f), // Bottom left
		Vertex(-width, height, -depth,	0.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,	2.0f / 3.0f, 1.0f / 4.0f), // Top left
		Vertex(width, height, -depth,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f, 1.0f,	1.0f / 3.0f, 1.0f / 4.0f), // Top right
		Vertex(width, -height, -depth,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f, 0.0f,	1.0f / 3.0f, 0.0f / 4.0f), // Bottom right

		// Back
		Vertex(-width, -height, depth,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f), // Bottom left
		Vertex(width, -height, depth,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f), // Bottom right
		Vertex(width, height, depth,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f), // Top right
		Vertex(-width, height, depth,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f), // Top right

		// Top
		Vertex(-width, height, -depth,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f), // Top front left
		Vertex(-width, height, depth,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f), // Top back left
		Vertex(width, height, depth,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f), // Top back right
		Vertex(width, height, -depth,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f), // Top front left

		// Bottom
		Vertex(-width, -height, -depth,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f), // Bottom front left
		Vertex(width, -height, -depth,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f), // Bottom front right
		Vertex(width, -height, depth,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f), // Bottom back right
		Vertex(-width, -height, depth,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f), // Bottom back left

		// left
		Vertex(-width, -height, depth,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f),
		Vertex(-width, height, depth,		-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f),
		Vertex(-width, height, -depth,	-1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f),
		Vertex(-width, -height, -depth,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f),

		// Right
		Vertex(width, -height, -depth,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f),
		Vertex(width, height, -depth,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f),
		Vertex(width, height, depth,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f),
		Vertex(width, -height, depth,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f),
	};

	hr = pVertexBuffer->CreateVertexBuffer(gfx->GetDevice(), vertices, 0u);
	Logging::ThrowIf(hr, "Vertex Failed to build");
	pVertexBuffer->BindBuffer(0u, 1u, gfx->GetDeviceContext());

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
	hr = pIndexBuffer->CreateIndexBuffer(gfx->GetDevice(), indices);
	Logging::ThrowIf(hr, "Index failed to build");
	pIndexBuffer->BindBuffer(gfx->GetDeviceContext(), DXGI_FORMAT_R16_UINT, 0u);

	// Constant Buffer
	hr = pWVPconstBuffer->CreateStaticConstantBuffer(gfx->GetDevice());
	Logging::ThrowIf(hr, "const failed to build");

	hr = pPerObjectBuffer->CreateStaticConstantBuffer(gfx->GetDevice());
	Logging::ThrowIf(hr, "Cube's per object buffer failed to build");


	// Vertex Shader
	pVertexShader->ReadVSShaderToBlob(L"..\\x64\\Debug\\VertexShader.cso");
	pVertexShader->CreateVSShader(gfx->GetDevice());

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	gfx->GetDevice()->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pVertexShader->GetVSBlob()->GetBufferPointer(),
		pVertexShader->GetVSBlob()->GetBufferSize(),
		&pInputLayout);


	// Pixel Shader
	hr = pPixelShader->ReadPSShaderToBlob(L"..\\x64\\Debug\\PixelShader.cso");
	Logging::ThrowIf(hr, "Pixel failed to read");

	hr = pPixelShader->CreatePSShader(gfx->GetDevice());
	Logging::ThrowIf(hr, "pixel failed to create");


	int img_w, img_h, num_channel;

	stbi_uc* texture = stbi_load("Assets\\Texture\\icon.png", &img_w, &img_h, &num_channel, STBI_rgb_alpha);

	if (stbi_failure_reason())
	{
		Logging::LogError("stbi_load failed to find texture");
	}

	D3D11_TEXTURE2D_DESC img_desc = {};
	img_desc.Width = img_w;
	img_desc.Height = img_h;
	img_desc.MipLevels = 1u;
	img_desc.ArraySize = 1;
	img_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	img_desc.SampleDesc.Count = 1u;
	img_desc.SampleDesc.Quality = 0u;
	img_desc.Usage = D3D11_USAGE_DEFAULT;
	img_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	img_desc.CPUAccessFlags = 0u;
	img_desc.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA img_data = {};
	img_data.pSysMem = texture;
	img_data.SysMemSlicePitch = width * height * num_channel;
	img_data.SysMemPitch = width * height * num_channel;

	gfx->GetDevice()->CreateTexture2D(&img_desc, &img_data, &pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = img_desc.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = -1;


	gfx->GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvd, &pShaderResourceView);


	D3D11_SAMPLER_DESC sampleDesc = {};
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.MinLOD = 1u;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	gfx->GetDevice()->CreateSamplerState(&sampleDesc, &pSampleState);

	gfx->GetDeviceContext()->PSSetSamplers(0, 1, &pSampleState);



	gfx->GetDeviceContext()->IASetInputLayout(pInputLayout.Get());
	gfx->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	pWVPconstBuffer->data.matrix = transform.GetWorldMatrix() * gfx->GetViewMatrix() * gfx->GetProjectionMatrix();
	pWVPconstBuffer->data.matrix = DirectX::XMMatrixTranspose(pWVPconstBuffer->data.matrix);

	// Update and set constant buffer
	pWVPconstBuffer->UpdateSubResource(gfx->GetDeviceContext());
	//hr = pVertConstBuffer->UpdateBuffer(gfx->GetDeviceContext());
	//Logging::ThrowIf(hr, "Constant buffer failed to update");
	
	pWVPconstBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), 0u, 1u);

	// Set Shaders
	pVertexShader->SetVSShader(gfx->GetDeviceContext(), 0u);
	pPixelShader->SetPSShader(gfx->GetDeviceContext(), 0u);

	gfx->GetDeviceContext()->PSSetShaderResources(0, 1, pShaderResourceView.GetAddressOf());

	// Draw
	gfx->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
}
