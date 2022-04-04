#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\matrix4x4.h>
#include <assimp\cimport.h>

#include <string>
#include <vector>

#include "Vertex.h"

class ModelLoader
{
	public:
		ModelLoader(std::string _file_path, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices, unsigned int pFlags);
		ModelLoader(std::string _file_path);
		~ModelLoader() = default;

		void Release();

		const std::vector<Vertex>& GetVertices() const;
		const std::vector<unsigned short>& GetIndices() const;

	private:
		Assimp::Importer importer;
		const aiScene* pScene;
		const aiMesh* pMesh;

		bool LoadMeshData(std::vector<Vertex>& vertices, std::vector<unsigned short>& indices);

		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

};
