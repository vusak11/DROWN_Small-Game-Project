#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H
#include "map.h"
#include "../Globals.h"
#include <fstream>
#include <string>
#include <cmath>
#include "../Map/metadata.h"
// Forward declaration

// This class is used as a handler of the map(s) and how it will work in the game.  
// Example:
//    handler->GetMap(0)->load_map();
//   


class MapHandler {
private:
	struct GridMap {
		Map map_cell;
		glm::vec3 translate;
		glm::vec2 cell_position;
	};

	MetaData meta_data_;

	std::vector<std::vector<float>>* map_data_;
	std::vector<std::vector<float>> test_data_;
	std::vector<std::vector<GridMap>> grid_map_;
	std::vector<glm::vec2> cells_to_draw_;
public:
	MapHandler();
	~MapHandler();

	void InitializeMaps(
		std::string map_path,
		std::string texture_path_0,
		std::string texture_path_1);
	void InitializeBuffers(GLuint shader);

	glm::mat4 Transformation(int column, int row);
	void Draw(GLuint shader, int column, int row);
	
	glm::vec2 CurrentCell(glm::vec3 players_current_position);
	std::vector<glm::vec2> GridCulling(glm::vec2 current_cell);

	std::vector<std::vector<float>>* GetMapDataPointer();
	std::string GetZone(glm::vec2 zone_central_points);
};

#endif