#include "Cube.h"
#include "ErrorChecker.h"
#include "Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


Cube::Cube(Graphics* gfx)
{
	HRESULT hr;

	loadModel();
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


	int img_w = 0;
	int img_h = 0;
	int num_channel = 0;

	stbi_uc* texture = stbi_load("Assets\\Texture\\icon.png", &img_w, &img_h, &num_channel, STBI_rgb);

	if (stbi_failure_reason())
	{
		Logging::LogError("stbi_load failed to find texture");
	}

	D3D11_TEXTURE2D_DESC img_desc = {};
	img_desc.Width = img_w;
	img_desc.Height = img_h;
	img_desc.MipLevels = 1u;
	img_desc.ArraySize = 1u;
	img_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	img_desc.SampleDesc.Count = 1u;
	img_desc.SampleDesc.Quality = 0u;
	img_desc.Usage = D3D11_USAGE_DEFAULT;
	img_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	img_desc.CPUAccessFlags = 0u;
	img_desc.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA img_data = {};
	img_data.pSysMem = texture;
	img_data.SysMemPitch = img_w * num_channel;

	hr = gfx->GetDevice()->CreateTexture2D(&img_desc, &img_data, &pTexture);
	Logging::ThrowIf(hr, "Failed to create 2D texture");

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

	// transform.SetRotationAxis(rot);
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
	// gfx->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
	gfx->GetDeviceContext()->Draw(static_cast<UINT>(vertices.size()), 0u);
}

void Cube::loadModel()
{
	const std::string model_path = "Assets\\Model\\cube.obj";

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, error;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, model_path.c_str()))
	{
		Logging::LogError(error);
	}

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex;

			vertex.position = 
			{ 
				attrib.vertices[3 * size_t(index.vertex_index) + 0],
				attrib.vertices[3 * size_t(index.vertex_index) + 1],
				attrib.vertices[3 * size_t(index.vertex_index) + 2]
			};

			vertex.normal =
			{
				attrib.normals[3 * size_t(index.normal_index) + 0],
				attrib.normals[3 * size_t(index.normal_index) + 1],
				attrib.normals[3 * size_t(index.normal_index) + 2]
			};

			vertex.texcoord =
			{
				attrib.texcoords[2 * size_t(index.texcoord_index) + 0],
				attrib.texcoords[2 * size_t(index.texcoord_index) + 1]
			};

			vertices.push_back(vertex);
		}
	}

}
