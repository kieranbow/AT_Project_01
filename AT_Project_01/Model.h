#pragma once
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "VSShader.h"
#include "PSShader.h"
#include "TransformComponent.h"

#include <string>

class ModelLoader;

class Model
{
	public:
		Model() = default;
		~Model() = default;

		void LoadMesh(Graphics* pGfx, std::string mesh_file_path);
		void LoadShaders(Graphics* gfx, LPCWSTR vs_file_path, LPCWSTR ps_file_path, D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements);
		void Update(float dt);
		void Draw(Graphics* gfx);

		TransformComponent transform;

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		std::vector<Mesh> meshes;

		std::unique_ptr<ConstantBuffer<PerObject>> pWorldBuffer = std::make_unique<ConstantBuffer<PerObject>>();
		std::unique_ptr<ConstantBuffer<PerFrame>> pFrameBuffer = std::make_unique<ConstantBuffer<PerFrame>>();

		std::unique_ptr<VSShader> pVertexShader = std::make_unique<VSShader>();
		std::unique_ptr<PSShader> pPixelShader = std::make_unique<PSShader>();

};

