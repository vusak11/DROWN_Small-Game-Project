#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H
#include "map.h"
#include <fstream>
#include <string>
#include <cmath>
// Forward declaration

// This class is used as a handler of the map(s) and how it will work in the game.  
// Example:
//    handler->GetMap(0)->load_map();
//   


class MapHandler {
private:
	//Map* maps_;	// A list of all the maps in the current game.
	struct GridMap {
		Map map_cell_;
		glm::vec3 translate_;
		glm::vec2 cell_position_;
	};

	std::vector<std::vector<GridMap>> grid_map_;
	std::vector<glm::vec2> cells_to_draw_;
	//int* cells_to_draw_;
	
	float grid_column_;
	float grid_row_;
	
public:
	MapHandler();
	~MapHandler();

	void InitializeMaps(std::string map_path, std::string texture_path);
	void InitializeBuffers(GLuint shader);

	glm::mat4 Transformation(int i, int j);
	void Draw(GLuint shader, int i, int j);
	
	glm::vec2 CurrentCell(glm::vec3 players_current_position);
	std::vector<glm::vec2> GridCulling(glm::vec2 current_cell);
};

#endif