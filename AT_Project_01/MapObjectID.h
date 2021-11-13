#pragma once
#include <string>
struct ObjectID : std::string
{
	std::string Air_block		= "000:0";

	// Floor
	std::string Floor				= "001:0";
	std::string Floor_1_airBlock	= "001:1";
	std::string Floor_2_airBlock	= "001:2";
	std::string Floor_3_airBlock	= "001:3";
	std::string Floor_4_airBlock	= "001:4";
	std::string Floor_5_airBlock	= "001:5";
	std::string Floor_6_airBlock	= "001:6";
	std::string Floor_7_airBlock	= "001:7";
	std::string Floor_8_airBlock	= "001:8";
	std::string Floor_9_airBlock	= "001:9";

	// Wall
	std::string Wall				= "002:0";
	std::string Wall_1_high			= "002:1";
	std::string Wall_2_high			= "002:2";
	std::string Wall_3_high			= "002:3";
	std::string Wall_4_high			= "002:4";
	std::string Wall_5_high			= "002:5";
	std::string Wall_6_high			= "002:6";
	std::string Wall_7_high			= "002:7";
	std::string Wall_8_high			= "002:8";
	std::string Wall_9_high			= "002:9";

	// Ceiling
	std::string Ceiling				= "001:0";
	std::string Ceiling_1_airBlock	= "001:1";
	std::string Ceiling_2_airBlock	= "001:2";
	std::string Ceiling_3_airBlock	= "001:3";
	std::string Ceiling_4_airBlock	= "001:4";
	std::string Ceiling_5_airBlock	= "001:5";
	std::string Ceiling_6_airBlock	= "001:6";
	std::string Ceiling_7_airBlock	= "001:7";
	std::string Ceiling_8_airBlock	= "001:8";
	std::string Ceiling_9_airBlock	= "001:9";
};
