#pragma once
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "VSShader.h"
#include "PSShader.h"
#include "TransformComponent.h"
#include "Texture.h"

#include <string>

class ModelLoader;

class Model
{
	public:
		Model() = default;
		~Model() = default;

		void LoadMeshFromSource(Graphics* pGfx, std::string mesh_file_path);
		void LoadMesh(Graphics* pGfx, std::vector<Vertex> _vertices, std::vector<unsigned short> _indices);
		void LoadShaders(Graphics* gfx, LPCWSTR vs_file_path, LPCWSTR ps_file_path, D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements);
		void LoadTextures(Graphics* gfx, std::string texture_file_path);
		void Update(float dt);
		void Draw(Graphics* gfx);

		TransformComponent transform;

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		std::vector<Mesh> meshes;
		std::vector<Texture> textures;

		std::unique_ptr<ConstantBuffer<PerObject>> pWorldBuffer = std::make_unique<ConstantBuffer<PerObject>>();
		std::unique_ptr<ConstantBuffer<PerFrame>> pFrameBuffer = std::make_unique<ConstantBuffer<PerFrame>>();

		std::unique_ptr<VSShader> pVertexShader = std::make_unique<VSShader>();
		std::unique_ptr<PSShader> pPixelShader = std::make_unique<PSShader>();

};
