#include "Model.h"
#include "Model_loader.h"
#include "Graphics.h"
void Model::LoadMesh(Graphics* pGfx, std::string mesh_file_path)
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

	// Create static constant buffers for perObject and perFrame
	pWorldBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	pFrameBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());

	// Loop through meshes and bind their buffers to the pipeline
	for (auto& mesh : meshes)
	{
		mesh.Bind(pGfx);
	}
}

void Model::LoadShaders(Graphics* gfx, LPCWSTR vs_file_path, LPCWSTR ps_file_path, D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements)
{
	// Read and Create vertex shader
	pVertexShader->ReadVSShaderToBlob(vs_file_path);
	pVertexShader->CreateVSShader(gfx->GetDevice(), pLayoutDesc, numElements);

	// Read and create pixel shader
	pPixelShader->ReadPSShaderToBlob(ps_file_path);
	pPixelShader->CreatePSShader(gfx->GetDevice());

	// Set inputlayout and primitivetopology
	gfx->GetDeviceContext()->IASetInputLayout(pVertexShader->GetInputLayout());
	gfx->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Model::Update(float dt)
{
	transform.Update();
}

void Model::Draw(Graphics* gfx)
{
	for (auto& mesh : meshes)
	{
		mesh.Bind(gfx);
		// Update world buffer and set bind it to vertex shader
		pWorldBuffer->data.m_world = transform.GetWorldMatrix();
		pWorldBuffer->UpdateSubResource(gfx->GetDeviceContext());
		pWorldBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), 0u, 1); //b0

		// Update frame buffer and set bind it to vertex shader
		pFrameBuffer->data.m_view = gfx->GetViewMatrix();
		pFrameBuffer->data.m_projection = gfx->GetProjectionMatrix();
		pFrameBuffer->UpdateSubResource(gfx->GetDeviceContext());
		pFrameBuffer->SetVSConstBuffer(gfx->GetDeviceContext(), 1u, 1); //b1

		// Set shaders
		pVertexShader->SetVSShader(gfx->GetDeviceContext(), 0u);
		pPixelShader->SetPSShader(gfx->GetDeviceContext(), 0u);

		// Draw
		gfx->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
	}
}
