#pragma once
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"
#include "VSShader.h"
#include "PSShader.h"
#include "Texture.h"
#include "Material.h"
#include <string>

class ModelLoader;
class TransformComponent;

class ModelComponent
{
	public:
		ModelComponent();
		ModelComponent(TransformComponent* pTransform);
		~ModelComponent() = default;

		// Using Assimp, it reads and loads the mesh from the file and stores data into buffers
		void LoadMeshFromSource(Graphics* pGfx, std::string mesh_file_path);

		// If data like vertices and indices are already present, this will create the mesh by using Mesh's constuctor
		void LoadMesh(Graphics* pGfx, std::vector<Vertex> _vertices, std::vector<unsigned short> _indices);

		// When the vertex and pixel shaders are provided. This will read and create each shader and set the input layout.
		void LoadShaders(Graphics* pGfx, std::wstring vs_filename, std::wstring ps_filename, D3D11_INPUT_ELEMENT_DESC* pInputDesc, UINT numElements);

		// Textures are loaded using Texture class functions
		void LoadTextures(Graphics* pGfx, std::string str_texture_file_path, DXGI_FORMAT format);


		void LoadTextureUsingTextureData(Graphics* pGfx, TextureData textureData, DXGI_FORMAT format);

		// Update models position/scale/rotation using the transform componment
		void Update(float dt);

		// All buffers, constants buffers and shaders are set and then drawn.
		void Draw(Graphics* gfx);

		void SetBlinnPhongMaterial(Material_BlinnPhong mat);
		Material_BlinnPhong GetBlinnPhongMaterial() const;

		void SetPBRMaterial(Material_PBR mat);
		Material_PBR GetPBRMaterial() const;

		// Sets the position using the transform componment
		void SetPosition(DirectX::XMFLOAT3 position);
		DirectX::XMFLOAT3 GetPosition() const;

		// Sets the rotation using the transform componment
		void SetRotation(DirectX::XMFLOAT3 rotation);
		DirectX::XMFLOAT3 GetRotation() const;

		// Sets the scale using the transform componment
		void SetScale(DirectX::XMFLOAT3 scale);
		DirectX::XMFLOAT3 GetScale() const;

		// Enables the rendering of the model. By default its enabled.
		void enableRendering();
		void disableRendering();
		const bool& getRenderStatus() const;

	private:
		// Mesh data
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;
		std::vector<Mesh> meshes;
		std::vector<Texture> textures;

		// Components
		TransformComponent* transform;

		// Material
		Material_BlinnPhong BP_material;
		Material_PBR PBR_material;

		bool isUsingTexture = false;
		bool isUsingPBRMat = false;
		bool m_enable = true;

		// Constant Buffers
		std::unique_ptr<ConstantBuffer<WorldViewProj>> pWVPbuffer			= std::make_unique<ConstantBuffer<WorldViewProj>>();
		std::unique_ptr<ConstantBuffer<PerFrame>> pFrameBuffer				= std::make_unique<ConstantBuffer<PerFrame>>();
		std::unique_ptr<ConstantBuffer<Material_BlinnPhong>> pBPMatBuffer	= std::make_unique<ConstantBuffer<Material_BlinnPhong>>();
		std::unique_ptr<ConstantBuffer<Material_PBR>> pPBRMatBuffer			= std::make_unique<ConstantBuffer<Material_PBR>>();
		// Shaders
		std::unique_ptr<VSShader> pVertexShader = std::make_unique<VSShader>();
		std::unique_ptr<PSShader> pPixelShader	= std::make_unique<PSShader>();
};
