#pragma once
#include <fstream>
#include <map>

#include "MapObjectID.h"

#include "Model.h"

class Map
{
public:
	Map() = default;
	~Map() = default;

	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;

	bool LoadMap(std::string file_path);

	void Update(float dt);
	void Draw();

private:
	std::map<ObjectID, Model> map;


	std::vector<std::unique_ptr<Model>> objects;
};
