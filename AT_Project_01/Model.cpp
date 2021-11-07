#include "Model.h"
#include "Model_loader.h"
#include "Graphics.h"

void Model::LoadMeshFromSource(Graphics* pGfx, std::string mesh_file_path)
{
	// Load mesh using Assimp
	ModelLoader loader(
		mesh_file_path,
		vertices, 
		indices,
		aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
		| aiProcess_Triangulate);

	// Add mesh to vector of meshes
	meshes.push_back(Mesh(pGfx, vertices, indices));

	// Create static constant buffers
	pWorldBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	//pFrameBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	pPixelBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());

	// Loop through meshes and bind their buffers to the pipeline
	for (auto& mesh : meshes)
	{
		mesh.Bind(pGfx);
	}
}

void Model::LoadMesh(Graphics* pGfx, std::vector<Vertex> _vertices, std::vector<unsigned short> _indices)
{
	vertices = _vertices;
	indices = _indices;

	// Add mesh to vector of meshes
	meshes.push_back(Mesh(pGfx, vertices, indices));

	// Create static constant buffers for perObject and perFrame
	pWorldBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	//pFrameBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	pPixelBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());

	// Loop through meshes and bind their buffers to the pipeline
	for (auto& mesh : meshes)
	{
		mesh.Bind(pGfx);
	}
}

void Model::LoadShaders(Graphics* gfx, LPCWSTR vs_file_path, LPCWSTR ps_file_path, D3D11_INPUT_ELEMENT_DESC* pInputDesc, UINT numElements)
{
	// Read and Create vertex shader
	pVertexShader->ReadVSShaderToBlob(vs_file_path);
	pVertexShader->CreateVSShader(gfx->GetDevice(), pInputDesc, numElements);

	// Read and create pixel shader
	pPixelShader->ReadPSShaderToBlob(ps_file_path);
	pPixelShader->CreatePSShader(gfx->GetDevice());

	// Set inputlayout and primitivetopology
	gfx->GetDeviceContext()->IASetInputLayout(pVertexShader->GetInputLayout());
	gfx->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::LoadTextures(Graphics* pGfx, std::string str_texture_file_path)
{
	Texture texture(pGfx);

	if(!texture.LoadAndCreateTexture(str_texture_file_path))
	{
		OutputDebugStringA("didn load texture");
	}
	
	if(!texture.CreateSampleState(0u, 1u))
	{
		OutputDebugStringA("didn create sample");
	}

	textures.push_back(texture);
}

void Model::Update(float dt)
{
	transform.Update();
}

void Model::Draw(Graphics* gfx)
{
	for (auto& mesh : meshes)
	{
		// Bind Vertex and Index buffers to pipeline
		mesh.Bind(gfx);

		// Update world buffer and bind it to vertex shader
		pWorldBuffer->data.m_world = transform.GetWorldMatrix();
		pWorldBuffer->data.m_view = gfx->GetViewMatrix();
		pWorldBuffer->data.m_projection = gfx->GetProjectionMatrix();
		pWorldBuffer->UpdateSubResource(gfx->GetDeviceContext());
		pWorldBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), Bind::Buffer::b0, 1u); //b0

		// Update frame buffer and set bind it to vertex shader
		//pFrameBuffer->data.m_view = gfx->GetViewMatrix();
		//pFrameBuffer->data.m_projection = gfx->GetProjectionMatrix();
		//pFrameBuffer->UpdateSubResource(gfx->GetDeviceContext());
		//pFrameBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), Bind::Buffer::b1, 1); //b1

		// Update pixel buffer
		pPixelBuffer->data.lightColor = {1.0f, 1.0f, 1.0f };
		pPixelBuffer->data.lightStength = 0.1f;
		pPixelBuffer->data.dynamicLightPosition = {0.0f, 0.0f, 0.0f};
		pPixelBuffer->data.dynamicLightColor = {1.0f, 1.0f, 1.0f};
		pPixelBuffer->data.dynamicLightStrength = 10.0f;
		pPixelBuffer->UpdateSubResource(gfx->GetDeviceContext());
		pPixelBuffer->SetPSConstBuffer(gfx->GetDeviceContext(), Bind::Buffer::b0, 1u);

		// Set textures
		textures.at(0).SetShaderResource(Bind::Texture::t0, 1u);

		// Set shaders
		pVertexShader->SetVSShader(gfx->GetDeviceContext(), 0u);
		pPixelShader->SetPSShader(gfx->GetDeviceContext(), 0u);

		// Draw
		gfx->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
	}
}
