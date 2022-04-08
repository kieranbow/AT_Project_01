#include "Map.h"
#include "Graphics.h"
#include "Model_loader.h"
#include "Player.h"

Map::Map()
{
	// Air | Roof | Floor
	objectIDMap.emplace("0:0", mapObjectID::Air_block);
	objectIDMap.emplace("0:1", mapObjectID::Floor);
	objectIDMap.emplace("0:2", mapObjectID::Roof);
	objectIDMap.emplace("0:3", mapObjectID::Player_Spawn_Floor);
	objectIDMap.emplace("0:4", mapObjectID::Enemy_Spawn_Floor);
	objectIDMap.emplace("0:5", mapObjectID::Player_Spawn_FloorRoof);
	objectIDMap.emplace("0:6", mapObjectID::Enemy_Spawn_FloorRoof);

	// Wall
	objectIDMap.emplace("1:0", mapObjectID::Wall_PosY); //y+
	objectIDMap.emplace("1:1", mapObjectID::Wall_NegY); //y-
	objectIDMap.emplace("1:2", mapObjectID::Wall_PosX); //x+
	objectIDMap.emplace("1:3", mapObjectID::Wall_NegX); //x-

	// Wall Corner
	objectIDMap.emplace("2:0", mapObjectID::Corner);

	// Pillar
	objectIDMap.emplace("3:0", mapObjectID::Pillar);
}

bool Map::LoadMap(Graphics* pGfx, std::string file_path, int mapWidth, DirectX::XMVECTOR& playerPosition)
{
	ModelLoader floor("Assets\\Model\\Plane.obj");

	ModelLoader wallStart("Assets\\Model\\lvl\\Wall\\Mid\\mid_01.obj");
	ModelLoader wallMid("Assets\\Model\\lvl\\Wall\\Mid\\mid_02.obj");
	ModelLoader wallEnd("Assets\\Model\\cube_proj.obj");

	ModelLoader pillar_start("Assets\\Model\\pillar_start.obj");
	ModelLoader pillar_mid("Assets\\Model\\pillar_mid.obj");
	ModelLoader pillar_end("Assets\\Model\\pillar_end.obj");

	// Corner Wall
	ModelLoader corner("Assets\\Model\\cube_proj.obj");

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

	map_file.open(file_path);

	if (map_file.is_open())
	{
		// Stream every word from the file into word string
		while (map_file >> word)
		{
			if (x == mapWidth)
			{
				x = 0.0f;
				z += 2.0f;
			}

			// Using the word string, find the mesh based off its ID and create and place it.
			switch (FindObjectID(word))
			{
				case mapObjectID::Air_block:
					break;

				case mapObjectID::Floor:
				{
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, -1.0f, z }, { 0.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
				}
				break;

				case mapObjectID::Roof: // Roof and floor
				{
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, -1.0f, z }, { 0.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, 5.0f, z }, { 180.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
				}
				break;

				case mapObjectID::Player_Spawn_Floor:
				{
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, -1.0f, z }, { 0.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
					playerPosition = XMVectorSet(x, 2.0f, z, 0.0f);
				}
				break;

				case mapObjectID::Player_Spawn_FloorRoof:
				{
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, -1.0f, z }, { 0.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, 5.0f, z }, { 180.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
					playerPosition = XMVectorSet(x, 2.0f, z, 0.0f);
				}
				break;

				case mapObjectID::Enemy_Spawn_Floor:
				{
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, -1.0f, z }, { 0.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
					XMFLOAT3 position = { x, 0.0f, z };
					enemyPosition.push_back(position);
				}
				break;

				case mapObjectID::Enemy_Spawn_FloorRoof:
				{
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, -1.0f, z }, { 0.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, floorAlbedo, floorRMAO, { x, 5.0f, z }, { 180.0f, 0.0f, 0.0f }, floor.GetVertices(), floor.GetIndices());
					XMFLOAT3 position = { x, 0.0f, z };
					enemyPosition.push_back(position);
				}


				case mapObjectID::Wall_PosY:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, { 0.0f, 0.0f, 0.0f }, wallStart.GetVertices(), wallStart.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, { 0.0f, 0.0f, 0.0f }, wallMid.GetVertices(), wallMid.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 4.0f, z }, { 0.0f, 0.0f, 0.0f }, wallEnd.GetVertices(), wallMid.GetIndices());
				}
				break;

				case mapObjectID::Wall_NegY:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, { 0.0f, -180.0f, 0.0f }, wallStart.GetVertices(), wallStart.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, { 0.0f, -180.0f, 0.0f }, wallMid.GetVertices(), wallMid.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 4.0f, z }, { 0.0f, -180.0f, 0.0f }, wallEnd.GetVertices(), wallMid.GetIndices());
				}
				break;

				case mapObjectID::Wall_PosX:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, {0.0f, 90.0f, 0.0f}, wallStart.GetVertices(), wallStart.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, {0.0f, 90.0f, 0.0f}, wallMid.GetVertices(), wallMid.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 4.0f, z }, {0.0f, 90.0f, 0.0f}, wallEnd.GetVertices(), wallMid.GetIndices());
				}
				break;

				case mapObjectID::Wall_NegX:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, { 0.0f, -90.0f, 0.0f }, wallStart.GetVertices(), wallStart.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, { 0.0f, -90.0f, 0.0f }, wallMid.GetVertices(), wallMid.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 4.0f, z }, { 0.0f, -90.0f, 0.0f }, wallEnd.GetVertices(), wallMid.GetIndices());
				}
				break;

				case mapObjectID::Corner:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, { 0.0f, 0.0f, 0.0f }, corner.GetVertices(), corner.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, { 0.0f, 0.0f, 0.0f }, corner.GetVertices(), corner.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 4.0f, z }, { 0.0f, 0.0f, 0.0f }, corner.GetVertices(), corner.GetIndices());
				}
				break;

				case mapObjectID::Pillar:
				{
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 0.0f, z }, {0.0f, 0.0f, 0.0f}, pillar_start.GetVertices(), pillar_start.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 2.0f, z }, {0.0f, 0.0f, 0.0f}, pillar_mid.GetVertices(), pillar_mid.GetIndices());
					CreateMapObjectPBRFromLoader(pGfx, wallAlbedo, wallRMAO, { x, 4.0f, z }, {0.0f, 0.0f, 0.0f}, pillar_end.GetVertices(), pillar_end.GetIndices());

				}
				break;

				default:
					break;
			}
			x += 2.0f;
		}
		map_file.close();
		
		// When map is closed and finished loading, clear all data from loaded meshes
		floor.Release();

		wallStart.Release();
		wallMid.Release();
		wallEnd.Release();

		corner.Release();

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
	obj->pModel->LoadMeshFromSource(pGfx, model_filePath);
	obj->pModel->LoadShaders(pGfx, L"VS_Default.cso", L"PS_BlinnPhong.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
	obj->pModel->LoadTextures(pGfx, texture_filePath, DXGI_FORMAT_R8G8B8A8_UNORM);
	obj->pTransform->SetPosition(position.x, position.y, position.z);

	// Push object into object pool
	mapObjects.push_back(std::move(obj));

	// Delete the object memory
	obj.release();
}

void Map::CreateMapObjectPBR(Graphics* pGfx, std::string model_filePath, std::string albedo_filePath, std::string rmao_filePath, DirectX::XMFLOAT3 position)
{
	// Create object
	std::unique_ptr<DefaultObject> obj = std::make_unique<DefaultObject>();
	obj->pModel->LoadMeshFromSource(pGfx, model_filePath);
	obj->pModel->LoadShaders(pGfx, L"VS_Default.cso", L"PS_PBR.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
	obj->pModel->LoadTextures(pGfx, albedo_filePath, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
	obj->pModel->LoadTextures(pGfx, rmao_filePath, DXGI_FORMAT_R8G8B8A8_UNORM);
	obj->pTransform->SetPosition(position.x, position.y, position.z);

	// Push object into object pool
	mapObjects.push_back(std::move(obj));

	// Delete the object memory
	obj.release();
}

void Map::CreateMapObjectPBRFromLoader(Graphics* pGfx, const TextureData & albedo, const TextureData & rmao, const DirectX::XMFLOAT3 & position, DirectX::XMFLOAT3 rotation, const std::vector<Vertex>& _vertices, const std::vector<unsigned short>& _indices)
{
	auto degToRad = [=](float degree)
	{
		return degree * XM_PI / 180.0f;
	};

	// Create object
	std::unique_ptr<DefaultObject> obj = std::make_unique<DefaultObject>();
	obj->pModel->LoadMesh(pGfx, _vertices, _indices);
	obj->pModel->LoadShaders(pGfx, L"VS_Default.cso", L"PS_PBR.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());

	obj->pModel->LoadTextureUsingTextureData(pGfx, albedo, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
	obj->pModel->LoadTextureUsingTextureData(pGfx, rmao, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);

	obj->pTransform->SetPosition(position.x, position.y, position.z);
	obj->pTransform->SetScale(1.0f, 1.0f, 1.0f);
	obj->pTransform->SetRotation(degToRad(rotation.x), degToRad(rotation.y), degToRad(rotation.z));

	// Push object into object pool
	mapObjects.push_back(std::move(obj));

	// Delete the object memory
	obj.release();
}
