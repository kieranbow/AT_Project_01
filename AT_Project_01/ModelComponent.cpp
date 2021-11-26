#include "ModelComponent.h"
#include "Model_loader.h"
#include "Graphics.h"
#include "TransformComponent.h"

ModelComponent::ModelComponent()
{
	Material_BlinnPhong mat_default;
	mat_default.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mat_default.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mat_default.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	mat_default.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mat_default.SpecularPower = 10.0f;

	BP_material = mat_default;
}

ModelComponent::ModelComponent(TransformComponent* _pTransform)
{
	transform = _pTransform;

	Material_BlinnPhong mat_default;
	mat_default.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mat_default.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	mat_default.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	mat_default.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mat_default.SpecularPower = 10.0f;

	BP_material = mat_default;
}

void ModelComponent::LoadMeshFromSource(Graphics* pGfx, std::string mesh_file_path)
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
	pWVPbuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	pFrameBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());

	if (isUsingPBRMat)
	{
		pPBRMatBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	}
	else
	{
		pBPMatBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	}

	// Loop through meshes and bind their buffers to the pipeline
	for (auto& mesh : meshes)
	{
		mesh.Bind(pGfx);
	}
}

void ModelComponent::LoadMesh(Graphics* pGfx, std::vector<Vertex> _vertices, std::vector<unsigned short> _indices)
{
	vertices = _vertices;
	indices = _indices;

	// Add mesh to vector of meshes
	meshes.push_back(Mesh(pGfx, vertices, indices));

	// Create static constant buffers for perObject and perFrame
	pWVPbuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	pFrameBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	
	if (isUsingPBRMat)
	{
		pPBRMatBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	}
	else
	{
		pBPMatBuffer->CreateStaticConstantBuffer(pGfx->GetDevice());
	}

	// Loop through meshes and bind their buffers to the pipeline
	for (auto& mesh : meshes)
	{
		mesh.Bind(pGfx);
	}
}

void ModelComponent::LoadShaders(Graphics* gfx, LPCWSTR vs_file_path, LPCWSTR ps_file_path, D3D11_INPUT_ELEMENT_DESC* pInputDesc, UINT numElements)
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

void ModelComponent::LoadTextures(Graphics* pGfx, std::string str_texture_file_path, DXGI_FORMAT format)
{
	isUsingTexture = true;
	Texture texture(pGfx);

	if(!texture.LoadAndCreateTexture(str_texture_file_path, format))
	{
		OutputDebugStringA("didn load texture");
	}
	
	if(!texture.CreateSampleState(0u, 1u))
	{
		OutputDebugStringA("didn create sample");
	}

	textures.push_back(texture);
}

void ModelComponent::LoadTextureUsingTextureData(Graphics* pGfx, TextureData textureData, DXGI_FORMAT format)
{
	isUsingTexture = true;
	Texture texture(pGfx);

	if (!texture.CreateTextureFromTextureData(textureData, format))
	{
		OutputDebugStringA("didn load texture");
	}

	if (!texture.CreateSampleState(0u, 1u))
	{
		OutputDebugStringA("didn create sample");
	}

	textures.push_back(texture);
}

void ModelComponent::Update(float dt)
{
	transform->Update();
}

void ModelComponent::Draw(Graphics* gfx)
{
	for (auto& mesh : meshes)
	{
		// Bind Vertex and Index buffers to pipeline
		mesh.Bind(gfx);

		pWVPbuffer->data.m_world = DirectX::XMMatrixTranspose(transform->GetWorldMatrix());
		pWVPbuffer->data.m_view = DirectX::XMMatrixTranspose(gfx->GetViewMatrix());
		pWVPbuffer->data.m_projection = DirectX::XMMatrixTranspose(gfx->GetProjectionMatrix());
		pWVPbuffer->UpdateSubResource(gfx->GetDeviceContext());
		pWVPbuffer->SetVSConstBuffer(gfx->GetDeviceContext(), Bind::Buffer::b0, 1u);

		// 0.25f, 0.5f, -1.0f
		DirectX::XMStoreFloat4(&pFrameBuffer->data.eyePos, gfx->currentCamera.GetPosition());
		pFrameBuffer->data.light.direction = gfx->directionalLight.GetLightProperty().direction;
		pFrameBuffer->data.light.ambientColor = gfx->directionalLight.GetLightProperty().ambientColor;
		pFrameBuffer->data.light.color = gfx->directionalLight.GetLightProperty().color;
		pFrameBuffer->data.light.intensity = gfx->directionalLight.GetLightProperty().intensity;
		pFrameBuffer->UpdateSubResource(gfx->GetDeviceContext());
		pFrameBuffer->SetPSConstBuffer(gfx->GetDeviceContext(), Bind::Buffer::b0, 1u);


		if (isUsingPBRMat)
		{
			pPBRMatBuffer->data = PBR_material;
			pPBRMatBuffer->UpdateSubResource(gfx->GetDeviceContext());
			pPBRMatBuffer->SetPSConstBuffer(gfx->GetDeviceContext(), Bind::Buffer::b1, 1u);
		}
		else
		{
			pBPMatBuffer->data = BP_material;
			pBPMatBuffer->UpdateSubResource(gfx->GetDeviceContext());
			pBPMatBuffer->SetPSConstBuffer(gfx->GetDeviceContext(), Bind::Buffer::b1, 1u);
		}


		if (isUsingTexture)
		{
			UINT startSlot = 0u;
			for (auto& texture : textures)
			{
				// Set textures
				texture.SetShaderResource(startSlot, 1u);
				startSlot++;
			}
			
			//textures.at(0).SetShaderResource(Bind::Texture::t0, 1u);
		}

		// Set shaders
		pVertexShader->SetVSShader(gfx->GetDeviceContext(), 0u);
		pPixelShader->SetPSShader(gfx->GetDeviceContext(), 0u);

		// Draw
		gfx->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u);
	}
}

void ModelComponent::SetBlinnPhongMaterial(Material_BlinnPhong mat)
{
	BP_material = mat;
}

Material_BlinnPhong ModelComponent::GetBlinnPhongMaterial() const
{
	return BP_material;
}

void ModelComponent::SetPBRMaterial(Material_PBR mat)
{
	PBR_material = mat;
	isUsingPBRMat = true;
}

Material_PBR ModelComponent::GetPBRMaterial() const
{
	return PBR_material;
}

void ModelComponent::SetPosition(DirectX::XMFLOAT3 position)
{
	transform->SetPosition(position.x, position.y, position.z);
}

DirectX::XMFLOAT3 ModelComponent::GetPosition() const
{
	return transform->GetPosition();
}

void ModelComponent::SetRotation(DirectX::XMFLOAT3 rotation)
{
	transform->SetRotation(rotation.x, rotation.y, rotation.z);
}

DirectX::XMFLOAT3 ModelComponent::GetRotation() const
{
	return transform->GetRotation();
}

void ModelComponent::SetScale(DirectX::XMFLOAT3 scale)
{
	transform->SetScale(scale.x, scale.y, scale.z);
}

DirectX::XMFLOAT3 ModelComponent::GetScale() const
{
	return transform->GetScale();
}
