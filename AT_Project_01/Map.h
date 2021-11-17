#pragma once
#include <fstream>
#include <map>

#include "MapObjectID.h"

// Game Objects
#include "DefaultObject.h"

class Graphics;

class Map
{
public:
	Map();
	~Map() = default;

	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;

	bool LoadMap(Graphics* pGfx, std::string file_path);
	void Update(float dt);
	void Draw(Graphics* pGfx);

private:
	mapObjectID FindObjectID(std::string id);
	void CreateMapObject(Graphics* pGfx, std::string model_filePath, std::string texture_filePath, DirectX::XMFLOAT3 position);
	void CreateMapObjectPBR(Graphics* pGfx, std::string model_filePath, std::string albedo_filePath, std::string rmao_filePath, DirectX::XMFLOAT3 position);

	std::map<std::string, mapObjectID> objectIDMap;
	std::vector<std::unique_ptr<DefaultObject>> mapObjects;
};
