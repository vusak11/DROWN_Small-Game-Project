#include "map_handler.h"

MapHandler::MapHandler(){

}

MapHandler::~MapHandler(){

}

void MapHandler::InitializeMaps(
	std::string map_path,
	std::string texture_path_0,
	std::string texture_path_1) {
	// ancillary_map loads in the whole .bmp picture and creates a 2D vector out of it
	// Create a grid map out of ancillary_map and calculate UV coordinates, normals,
	// triangles, indices and translation for each cell.

	float width, height = 0;
	Map ancillary_map;
	ancillary_map.LoadMap(map_path.c_str());
	ancillary_map.LoadTexture(texture_path_0.c_str(), texture_path_1.c_str());
	test_data_ = ancillary_map.GetTestList();
	map_data_ = ancillary_map.GetTempHeightList();
	meta_data_.Initialize();
	GridMap grid_cell;
	std::vector<GridMap> grid_cells;

	for (int j = 0; j < GRID_ROW; j++) {	
		grid_cells.clear();		
		width = 0;
		for (int i = 0; i < GRID_COLUMN; i++) {
			ancillary_map.CreateCells(GRID_COLUMN, GRID_ROW, j, i);
			ancillary_map.UVCoordinates();
			ancillary_map.CalculateNormals();
			ancillary_map.CreateTriangles();
			ancillary_map.CreateIndices();

			grid_cell.map_cell = ancillary_map;
			grid_cell.map_cell.CleanMapInfo();
			grid_cell.translate.x = width;
			grid_cell.translate.y = height;
			grid_cell.translate.z = 0.0f;
			grid_cell.cell_position = glm::vec2(i, j);
			grid_cells.push_back(grid_cell);

			width += grid_cell.map_cell.GetCellWidth() - GRID_GAP;
		}
		height -= grid_cell.map_cell.GetCellHeight() - GRID_GAP;
		grid_map_.push_back(grid_cells);
	}
}

void MapHandler::InitializeBuffers(GLuint shader) {
	for (int j = 0; j < grid_map_.size(); j++) {
		for (int i = 0; i < grid_map_[j].size(); i++) {
			grid_map_[j][i].map_cell.Buffer(shader);
		}
	}
}

glm::mat4 MapHandler::Transformation(int column, int row) {
	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, grid_map_[row][column].translate);
	model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, MAP_DEPTH));
	return model_matrix;
}

void MapHandler::Draw(GLuint shader, int column, int row) {
	grid_map_[row][column].map_cell.Draw(shader);
}


glm::vec2 MapHandler::CurrentCell(glm::vec3 players_current_position) {
	// Finds out what grid cell player stands on
	float x, y = 0;
	for (int j = 0; j < grid_map_.size(); j++) {
		for (int i = 0; i < grid_map_[j].size(); i++) {
			if (players_current_position.x > grid_map_[j][i].translate.x)
				x = i;
			if (players_current_position.y < grid_map_[j][i].translate.y)
				y = j;
		}
	}

	return glm::vec2(x, y);
}

std::vector<glm::vec2> MapHandler::GridCulling(glm::vec2 current_cell) {
	// Check all neighbouring cells and see whether they exists or not.
	// If they do exist, push the location into cells_to_draw vector.
	// 'Center' will always be drawn because that's where the player stands
	cells_to_draw_.clear();
	if (current_cell.x == 0 && current_cell.y == 0) {							// Upper left corner
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));		// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));		// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y + 1));
	} else if (current_cell.x == (GRID_COLUMN - 1) && current_cell.y == 0) {	// Upper right corner
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));		// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y + 1));	// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
	} else if (current_cell.x == 0 && current_cell.y == (GRID_ROW - 1)) {		// Lower left corner
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y - 1));	// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
	} else if (current_cell.x == (GRID_COLUMN - 1) && current_cell.y == (GRID_ROW - 1)) {// Lower right corner
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y - 1));	// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));		// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));		
	} else if (current_cell.x > 0 && current_cell.x < GRID_COLUMN &&			// Upper side
		current_cell.y == 0) {															// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));		// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y + 1));
	} else if (current_cell.x == 0 && current_cell.y > 0 && current_cell.y < GRID_ROW) {// Left side
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y - 1));	// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y + 1));
	} else if (current_cell.x == (GRID_COLUMN - 1) &&							// Right side
		current_cell.y > 0 && current_cell.y < GRID_ROW) {								// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y - 1));	// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));	
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
	} else if (current_cell.x > 0 && current_cell.x < GRID_COLUMN &&			// Lower side
		current_cell.y == (GRID_ROW - 1)) {											// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y - 1));	// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y - 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
	} else if (current_cell.x > 0 && current_cell.y > 0 &&						// Inner
		current_cell.x < GRID_COLUMN && current_cell.y < GRID_ROW) {					// 1, 1 ,1
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y - 1));	// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y - 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y + 1));
	}
	return cells_to_draw_;
}

std::vector<std::vector<float>>* MapHandler::GetMapDataPointer()
{
	return &test_data_;
}

std::string MapHandler::GetZone(glm::vec2 zone_central_points) {
	return meta_data_.GetZone(zone_central_points);
}
