#pragma once
#include <string>
// An enum class containing all the IDs used for map objects
enum class mapObjectID
{
	Invalid,
	Air_block,
	Floor,

	Wall_PosY,
	Wall_NegY,
	Wall_PosX,
	Wall_NegX,

	Corner,

	Pillar,

	Roof,
	Player_Spawn_Floor,
	Player_Spawn_FloorRoof,
	Enemy_Spawn_Floor,
	Enemy_Spawn_FloorRoof
};
