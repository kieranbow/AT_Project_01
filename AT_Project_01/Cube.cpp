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

	// loadModel();
	loadmodelAss();
	hr = pVertexBuffer->CreateVertexBuffer(gfx->GetDevice(), vertices, 0u);
	Logging::ThrowIf(hr, "Vertex Failed to build");
	pVertexBuffer->BindBuffer(0u, 1u, gfx->GetDeviceContext());


	hr = pIndexBuffer->CreateIndexBuffer(gfx->GetDevice(), indices);
	Logging::ThrowIf(hr, "Index failed to build");
	pIndexBuffer->BindBuffer(gfx->GetDeviceContext(), DXGI_FORMAT_R16_UINT, 0u);




	// Constant Buffer
	hr = pWorldBuffer->CreateStaticConstantBuffer(gfx->GetDevice());
	Logging::ThrowIf(hr, "const failed to build");

	hr = pFrameBuffer->CreateStaticConstantBuffer(gfx->GetDevice());
	Logging::ThrowIf(hr, "Cube's per object buffer failed to build");






	// Vertex Shader
	pVertexShader->ReadVSShaderToBlob(L"..\\x64\\Debug\\VertexShader.cso");
	pVertexShader->CreateVSShader(gfx->GetDevice());

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
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

	stbi_uc* texture = stbi_load("Assets\\Texture\\default.png", &img_w, &img_h, &num_channel, STBI_rgb_alpha);

	if (stbi_failure_reason())
	{
		Logging::LogError("stbi_load failed to find texture");
	}

	D3D11_TEXTURE2D_DESC img_desc = {};
	img_desc.Width = img_w;
	img_desc.Height = img_h;
	img_desc.MipLevels = 1u;
	img_desc.ArraySize = 1u;
	img_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	img_desc.SampleDesc.Count = 1u;
	img_desc.SampleDesc.Quality = 0u;
	img_desc.Usage = D3D11_USAGE_DEFAULT;
	img_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	img_desc.CPUAccessFlags = 0u;
	img_desc.MiscFlags = 0u;

	// https://gamedev.net/forums/topic/673814-d3d11-texture-image-data-from-memory/5266103/
	D3D11_SUBRESOURCE_DATA img_data = {};
	img_data.pSysMem = texture;
	img_data.SysMemPitch = img_w * 4;

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

	//transform.SetRotationAxis(rot);
	// transform.SetScale(1.0f, 1.0f, 1.0f);
	// transform.SetPosition(0.0f, 0.0f, rot);
	transform.Update();
}

void Cube::Draw(Graphics* gfx)
{
	HRESULT hr;
	// SetIAVertex buffer

	// Assign constant buffer new data
	//pWVPconstBuffer->data.matrix = transform.GetWorldMatrix() * gfx->GetViewMatrix() * gfx->GetProjectionMatrix();
	//pWVPconstBuffer->data.matrix = DirectX::XMMatrixTranspose(pWVPconstBuffer->data.matrix);

	pWorldBuffer->data.m_world = transform.GetWorldMatrix();
	pFrameBuffer->data.m_view = gfx->GetViewMatrix();
	pFrameBuffer->data.m_projection = gfx->GetProjectionMatrix();


	pWorldBuffer->UpdateSubResource(gfx->GetDeviceContext());
	pWorldBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), 0u, 1); //b0

	pFrameBuffer->UpdateSubResource(gfx->GetDeviceContext());
	pFrameBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), 1u, 1); //b2

	// Update and set constant buffer
	//pWVPconstBuffer->UpdateSubResource(gfx->GetDeviceContext());
	//hr = pVertConstBuffer->UpdateBuffer(gfx->GetDeviceContext());
	//Logging::ThrowIf(hr, "Constant buffer failed to update");
	
	//pWVPconstBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), 0u, 1u);

	// Set Shaders
	pVertexShader->SetVSShader(gfx->GetDeviceContext(), 0u);
	pPixelShader->SetPSShader(gfx->GetDeviceContext(), 0u);

	gfx->GetDeviceContext()->PSSetShaderResources(0, 1, pShaderResourceView.GetAddressOf());

	// Draw
	gfx->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
}

void Cube::loadModel()
{
	const std::string model_path = "Assets\\Model\\cube_proj.obj";

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

void Cube::loadmodelAss()
{

	const std::string model_path = "Assets\\Model\\Helmet_paintable_v2.obj";

	pScene = importer.ReadFile(
		model_path,
		aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
		| aiProcess_Triangulate);

	if (pScene == NULL)
	{
		Logging::LogError("scene node is null");
	}

	
	for (UINT m = 0; m < pScene->mNumMeshes; m++)
	{
		pMesh = pScene->mMeshes[m];

		for (UINT vrtIdx = 0; vrtIdx < pMesh->mNumVertices; vrtIdx++)
		{
			Vertex vertex;

			vertex.position.x = pMesh->mVertices[vrtIdx].x;
			vertex.position.y = pMesh->mVertices[vrtIdx].y;
			vertex.position.z = pMesh->mVertices[vrtIdx].z;

			vertex.normal.x = pMesh->mNormals[vrtIdx].x;
			vertex.normal.y = pMesh->mNormals[vrtIdx].y;
			vertex.normal.z = pMesh->mNormals[vrtIdx].z;

			if (pMesh->HasTextureCoords(0))
			{
				vertex.texcoord.x = pMesh->mTextureCoords[0][vrtIdx].x;
				vertex.texcoord.y = pMesh->mTextureCoords[0][vrtIdx].y;
			}

			vertices.push_back(vertex);
		}

		for (UINT i = 0; i < pMesh->mNumFaces; i++)
		{
			aiFace* face = &pMesh->mFaces[i];

			if (face->mNumIndices == 3)
			{
				indices.push_back(face->mIndices[0]);
				indices.push_back(face->mIndices[1]);
				indices.push_back(face->mIndices[2]);
			}

		}
	}
}
