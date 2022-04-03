#include "Map.h"
#include "Graphics.h"
#include "Model_loader.h"

Map::Map()
{
	objectIDMap.emplace("0:0", mapObjectID::Air_block);
	objectIDMap.emplace("1:0", mapObjectID::Floor);
	objectIDMap.emplace("2:0", mapObjectID::Wall_1_high);
	objectIDMap.emplace("2:1", mapObjectID::Wall_2_high);
	objectIDMap.emplace("2:2", mapObjectID::Wall_3_high);
	objectIDMap.emplace("3:0", mapObjectID::Roof);
	objectIDMap.emplace("4:1", mapObjectID::Pillar_1_high);
	objectIDMap.emplace("4:2", mapObjectID::Pillar_2_high);
	objectIDMap.emplace("4:3", mapObjectID::Pillar_3_high);
}

bool Map::LoadMap(Graphics* pGfx, std::string file_path)
{
	ModelLoader floor("Assets\\Model\\Plane.obj");
	ModelLoader wall("Assets\\Model\\cube_proj.obj");
	ModelLoader pillar_start("Assets\\Model\\pillar_start.obj");
	ModelLoader pillar_mid("Assets\\Model\\pillar_mid.obj");
	ModelLoader pillar_end("Assets\\Model\\pillar_end.obj");

	TextureData wallAlbedo;
	TextureData wallRMAO;
	TextureData floorAlbedo;
	TextureData floorRMAO;

	Texture textureLoader(pGfx);

	wallAlbedo	= textureLoader.LoadTextureData("Assets\\Texture\\factory wall\\factory_wall_diff_1k.png");
	wallRMAO	= textureLoader.LoadTextureData("Assets\\Texture\\factory wall\\factory_wall_arm_1k.png");
	floorAlbedo = textureLoader.LoadTextureData("Assets\\Texture\\Grey tiles\\large_grey_tiles_diff_1k.png");
	floorRMAO	= textureLoader.LoadTextureData("Assets\\Texture\\Grey tiles\\large_grey_tiles_rmao_1k.png");

	std::fstream map_file;
	std::string word;

	float x = 0.0f;
	float z = 0.0f;
	float map_width = 56.0f; // width * 2

	map_file.open(file_path);

	if (map_file.is_open())
	{
		// Stream every word from the file into word string
		while (map_file >> word)
		{
			if (x == map_width)
			{
				x = 0.0f;
				z += 2.0f;
			}

			// Using the word string, find the mesh based off its ID and create and place it.
			switch (FindObjectID(word))
			{
				case mapObjectID::Air_block:
				{
					break;
				}
				case mapObjectID::Floor:
				{
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, -1.0f, z }, floor.GetVertices(), floor.GetIndices());
					break;
				}
				case mapObjectID::Wall_1_high:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, wall.GetVertices(), wall.GetIndices());
					break;
				}
				case mapObjectID::Wall_2_high:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, wall.GetVertices(), wall.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, wall.GetVertices(), wall.GetIndices());	
					break;
				}
				case mapObjectID::Wall_3_high:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, wall.GetVertices(), wall.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, wall.GetVertices(), wall.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 4.0f, z }, wall.GetVertices(), wall.GetIndices());
					break;
				}

				case mapObjectID::Pillar_1_high:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, pillar_start.GetVertices(), pillar_start.GetIndices());
					break;
				}

				case mapObjectID::Pillar_2_high:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, pillar_start.GetVertices(), pillar_start.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, pillar_mid.GetVertices(), pillar_mid.GetIndices());
					break;
				}

				case mapObjectID::Pillar_3_high:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, pillar_start.GetVertices(), pillar_start.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, pillar_mid.GetVertices(), pillar_mid.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 4.0f, z }, pillar_end.GetVertices(), pillar_end.GetIndices());
					break;
				}

				case mapObjectID::Roof: // Roof and floor
				{
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, -1.0f, z }, floor.GetVertices(), floor.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, 4.0f, z }, wall.GetVertices(), wall.GetIndices());
					break;
				}

				default:
					break;
			}
			x += 2.0f;
		}
		map_file.close();
		
		// When map is closed and finished loading, clear all data from loaded meshes
		floor.Release();
		wall.Release();
		pillar_start.Release();
		pillar_mid.Release();
		pillar_end.Release();
	}
	return true;
}

void Map::Update(float dt)
{
	for(auto& object : mapObjects)
	{
		object->Update(dt);
	}
}

void Map::Draw(Graphics* pGfx)
{
	for (auto& object : mapObjects)
	{
		object->Draw(pGfx);
	}
}

mapObjectID Map::FindObjectID(std::string id)
{
	auto iter = objectIDMap.find(id);

	if (iter != objectIDMap.end())
	{
		return iter->second;
	}
	return mapObjectID::Invalid;
}

void Map::CreateMapObject(Graphics* pGfx, std::string model_filePath, std::string texture_filePath, DirectX::XMFLOAT3 position)
{
	// Create object
	std::unique_ptr<DefaultObject> obj = std::make_unique<DefaultObject>();
	obj->model->LoadMeshFromSource(pGfx, model_filePath);
	obj->model->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
	obj->model->LoadTextures(pGfx, texture_filePath, DXGI_FORMAT_R8G8B8A8_UNORM);
	obj->transform->SetPosition(position.x, position.y, position.z);

	// Push object into object pool
	mapObjects.push_back(std::move(obj));

	// Delete the object memory
	obj.release();
}

void Map::CreateMapObjectPBR(Graphics* pGfx, std::string model_filePath, std::string albedo_filePath, std::string rmao_filePath, DirectX::XMFLOAT3 position)
{
	// Create object
	std::unique_ptr<DefaultObject> obj = std::make_unique<DefaultObject>();
	obj->model->LoadMeshFromSource(pGfx, model_filePath);
	obj->model->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBR.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
	obj->model->LoadTextures(pGfx, albedo_filePath, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
	obj->model->LoadTextures(pGfx, rmao_filePath, DXGI_FORMAT_R8G8B8A8_UNORM);
	obj->transform->SetPosition(position.x, position.y, position.z);

	// Push object into object pool
	mapObjects.push_back(std::move(obj));

	// Delete the object memory
	obj.release();
}

void Map::CreateMapObjectPBRFromLoader(Graphics* pGfx, TextureData albedo, TextureData rmao, DirectX::XMFLOAT3 position, std::vector<Vertex> _vertices, std::vector<unsigned short> _indices)
{
	// Create object
	std::unique_ptr<DefaultObject> obj = std::make_unique<DefaultObject>();
	obj->model->LoadMesh(pGfx, _vertices, _indices);
	obj->model->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBR.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());

	obj->model->LoadTextureUsingTextureData(pGfx, albedo, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
	obj->model->LoadTextureUsingTextureData(pGfx, rmao, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);

	obj->transform->SetPosition(position.x, position.y, position.z);
	obj->transform->SetScale(1.0f, 1.0f, 1.0f);

	// Push object into object pool
	mapObjects.push_back(std::move(obj));

	// Delete the object memory
	obj.release();
}
