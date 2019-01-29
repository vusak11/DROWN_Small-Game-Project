#pragma once

#include <GLM/vec4.hpp>

// This class is symbolising the a map in the game and all its proporties.  
// Example:
//    map->GetMapWidth();
//  


class Map
{
public:
	Map();
	~Map();

	// This function is not complete.
	bool LoadMap(char* textureName);	// Returns a bool to trouble shoot while loading new maps.

	void SetHeight(int height);
	void SetWidth(int width);

	int GetHeight();
	int GetWidth();
private:
	int mMapWidth_;
	int mMapHeight_;
	glm_vec4* mMapData_;
};
