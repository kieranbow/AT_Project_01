#include "Map.h"
#include "Graphics.h"

Map::Map()
{
	// Initalize
	objectIDMap.emplace("0:0", mapObjectID::Air_block);
	objectIDMap.emplace("1:0", mapObjectID::Floor);
	objectIDMap.emplace("2:0", mapObjectID::Wall_1_high);
	objectIDMap.emplace("2:1", mapObjectID::Wall_2_high);
	objectIDMap.emplace("3:0", mapObjectID::Roof);
}

bool Map::LoadMap(Graphics* pGfx, std::string file_path)
{
	std::fstream map_file;
	std::string word;

	float x = 0.0f;
	float z = 0.0f;
	float map_width = 32.0f; // width * 2

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

			switch (FindObjectID(word))
			{
				case mapObjectID::Air_block:
				{
					break;
				}
				case mapObjectID::Floor:
				{
					CreateMapObject(pGfx, "Assets\\Model\\Plane.obj", { x, -1.0f, z });
					break;
				}
				case mapObjectID::Wall_1_high:
				{
					CreateMapObject(pGfx, "Assets\\Model\\cube_proj.obj", { x, 0.0f, z });
					break;
				}
				case mapObjectID::Wall_2_high:
				{
					CreateMapObject(pGfx, "Assets\\Model\\cube_proj.obj", { x, 0.0f, z });
					CreateMapObject(pGfx, "Assets\\Model\\cube_proj.obj", { x, 2.0f, z });
					break;
				}

				default:
					break;
			}
			x += 2.0f;
		}
		map_file.close();
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

void Map::CreateMapObject(Graphics* pGfx, std::string model_filePath, DirectX::XMFLOAT3 position)
{
	// Create object
	std::unique_ptr<DefaultObject> obj = std::make_unique<DefaultObject>();
	obj->model->LoadMeshFromSource(pGfx, model_filePath);
	obj->model->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_Floor.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
	obj->transform->SetPosition(position.x, position.y, position.z);

	// Push object into object pool
	mapObjects.push_back(std::move(obj));

	// Delete the object memory
	obj.release();
}
