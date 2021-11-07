#include "Model_loader.h"
#include "ErrorChecker.h"

ModelLoader::ModelLoader(std::string _file_path, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices, unsigned int pFlags)
{
	// Read file
	pScene = importer.ReadFile(_file_path, pFlags);

	if (pScene == NULL)
	{
		Logging::LogError("Assimp failed to read mesh file");
	}

	if (!LoadMeshData(vertices, indices))
	{
		Logging::LogError("Assimp failed to create mesh from file");
	}
}

ModelLoader::ModelLoader(std::string _file_path)
{
	// Read file
	pScene = importer.ReadFile(
		_file_path, 
		aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
		| aiProcess_Triangulate);

	if (pScene == NULL)
	{
		Logging::LogError("Assimp failed to read mesh file");
	}

	if (!LoadMeshData(vertices, indices))
	{
		Logging::LogError("Assimp failed to create mesh from file");
	}
}

std::vector<Vertex> ModelLoader::GetVertices() const
{
	return vertices;
}

std::vector<unsigned short> ModelLoader::GetIndices() const
{
	return indices;
}

bool ModelLoader::LoadMeshData(std::vector<Vertex>& vertices, std::vector<unsigned short>& indices)
{
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
	return true;
}
