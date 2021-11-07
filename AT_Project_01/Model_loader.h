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
		~ModelLoader() = default;

		ModelLoader(const ModelLoader&) = delete;
		ModelLoader& operator=(const ModelLoader&) = delete;

	private:
		Assimp::Importer importer;
		const aiScene* pScene;
		const aiMesh* pMesh;

		bool LoadMeshData(std::vector<Vertex>& vertices, std::vector<unsigned short>& indices);

};
