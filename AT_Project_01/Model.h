#pragma once
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"
#include "VSShader.h"
#include "PSShader.h"
#include "TransformComponent.h"
#include "Texture.h"
#include "Material.h"
#include <string>

class ModelLoader;

class Model
{
	public:
		Model();
		~Model() = default;

		// Using Assimp, it reads and loads the mesh from the file and stores data into buffers
		void LoadMeshFromSource(Graphics* pGfx, std::string mesh_file_path);

		// If data like vertices and indices are already present, this will create the mesh by using Mesh's constuctor
		void LoadMesh(Graphics* pGfx, std::vector<Vertex> _vertices, std::vector<unsigned short> _indices);

		// When the vertex and pixel shaders are provided. This will read and create each shader and set the input layout.
		void LoadShaders(Graphics* pGfx, LPCWSTR vs_file_path, LPCWSTR ps_file_path, D3D11_INPUT_ELEMENT_DESC* pInputDesc, UINT numElements);

		// Textures are loaded using Texture class functions
		void LoadTextures(Graphics* pGfx, std::string str_texture_file_path);

		// Update all models components.
		void Update(float dt);

		// All buffers, constants buffers and shaders are set and then drawn.
		void Draw(Graphics* gfx);

		TransformComponent transform;

		void SetMaterial(Material mat);

		Material GetMaterial() const;

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		std::vector<Mesh> meshes;
		std::vector<Texture> textures;
		Material material;

		std::unique_ptr<ConstantBuffer<WorldViewProj>> pWVPbuffer = std::make_unique<ConstantBuffer<WorldViewProj>>();
		std::unique_ptr<ConstantBuffer<PerFrame>> pFrameBuffer = std::make_unique<ConstantBuffer<PerFrame>>();
		std::unique_ptr<ConstantBuffer<MaterialProperties>> pMatBuffer = std::make_unique<ConstantBuffer<MaterialProperties>>();

		std::unique_ptr<VSShader> pVertexShader = std::make_unique<VSShader>();
		std::unique_ptr<PSShader> pPixelShader = std::make_unique<PSShader>();

		bool isUsingTexture = false;
};
