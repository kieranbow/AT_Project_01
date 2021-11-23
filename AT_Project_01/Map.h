#pragma once
// C++
#include <fstream>
#include <map>

// Game Objects
#include "DefaultObject.h"

#include "MapObjectID.h"

class Graphics;

// A map that can load the objects with a layout of the map from a text file
class Map
{
public:
	Map();
	~Map() = default;

	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;

	// Loads a map using text file. Returns false if map failed to load.
	bool LoadMap(Graphics* pGfx, std::string file_path);

	// Updates all map objects
	void Update(float dt);

	// Draws all map objects
	void Draw(Graphics* pGfx);

private:
	// Returns an enum using a string
	mapObjectID FindObjectID(std::string id);

	// Creates a map object that uses BlinnPhong rendering
	void CreateMapObject(Graphics* pGfx, std::string model_filePath, std::string texture_filePath, DirectX::XMFLOAT3 position);

	// Creates a map object that uses PBR rendering
	void CreateMapObjectPBR(Graphics* pGfx, std::string model_filePath, std::string albedo_filePath, std::string rmao_filePath, DirectX::XMFLOAT3 position);

	void CreateMapObjectPBRFromLoader(Graphics* pGfx, std::string albedo_filePath, std::string rmao_filePath, DirectX::XMFLOAT3 position, std::vector<Vertex> _vertices, std::vector<unsigned short> _indices);

	std::map<std::string, mapObjectID> objectIDMap;
	std::vector<std::unique_ptr<DefaultObject>> mapObjects;
};
