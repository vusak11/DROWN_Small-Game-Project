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
	GridMap grid_cell;
	std::vector<GridMap> grid_cells;

	int grid_row = (int)GlobalSettings::Access()->ValueOf("GRID_ROW");
	int grid_column = (int)GlobalSettings::Access()->ValueOf("GRID_COLUMN");
	float grid_gap = GlobalSettings::Access()->ValueOf("GRID_GAP");

	for (int j = 0; j < grid_row; j++) {
		grid_cells.clear();		
		width = 0;
		for (int i = 0; i < grid_column; i++) {
			ancillary_map.CreateCells(grid_column, grid_row, j, i);
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

			width += grid_cell.map_cell.GetCellWidth() - grid_gap;
		}
		height -= grid_cell.map_cell.GetCellHeight() - grid_gap;
		grid_map_.push_back(grid_cells);
	}
}

void MapHandler::InitializeBuffers(GLuint shader) {
	for (unsigned int j = 0; j < grid_map_.size(); j++) {
		for (unsigned int i = 0; i < grid_map_[j].size(); i++) {
			grid_map_[j][i].map_cell.Buffer(shader);
		}
	}
}

glm::mat4 MapHandler::Transformation(int column, int row) {
	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, grid_map_[row][column].translate);
	model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, GlobalSettings::Access()->ValueOf("MAP_DEPTH")));
	return model_matrix;
}

void MapHandler::Draw(GLuint shader, int column, int row) {
	grid_map_[row][column].map_cell.Draw(shader);
}


glm::vec2 MapHandler::CurrentCell(glm::vec3 players_current_position) {
	// Finds out what grid cell player stands on
	unsigned int x, y = 0;
	for (unsigned int j = 0; j < grid_map_.size(); j++) {
		for (unsigned int i = 0; i < grid_map_[j].size(); i++) {
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
	int grid_column = (int)GlobalSettings::Access()->ValueOf("GRID_COLUMN");
	int grid_row = (int)GlobalSettings::Access()->ValueOf("GRID_ROW");

	cells_to_draw_.clear();
	if (current_cell.x >= 0 && current_cell.x <= 4 &&
		current_cell.y >= 16 && current_cell.y <= 19) {
		for (int i = 0; i < 5; i++) {
			for (int j = 16; j < 20; j++) {
				cells_to_draw_.push_back(glm::vec2(i, j));
			}
		}
	} else if (current_cell.x == 0 && current_cell.y == 0) {							// Upper left corner
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));		// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));		// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y + 1));
	} else if (current_cell.x == (grid_column - 1) && current_cell.y == 0) {	// Upper right corner
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));		// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y + 1));	// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
	} else if (current_cell.x == 0 && current_cell.y == (grid_row - 1)) {		// Lower left corner
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y - 1));	// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
	} else if (current_cell.x == (grid_column - 1) && current_cell.y == (grid_row - 1)) {// Lower right corner
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y - 1));	// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));		// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));		
	} else if (current_cell.x > 0 && current_cell.x < grid_column &&			// Upper side
		current_cell.y == 0) {															// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));		// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y + 1));
	} else if (current_cell.x == 0 && current_cell.y > 0 && current_cell.y < grid_row) {// Left side
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y - 1));	// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));			// 0, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y + 1));
	} else if (current_cell.x == (grid_column - 1) &&							// Right side
		current_cell.y > 0 && current_cell.y < grid_row) {								// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y - 1));	// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 1, 1, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));	
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y + 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y + 1));
	} else if (current_cell.x > 0 && current_cell.x < grid_column &&			// Lower side
		current_cell.y == (grid_row - 1)) {											// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y - 1));	// 1, 1, 1
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y - 1));		// 0, 0, 0
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y - 1));
		cells_to_draw_.push_back(glm::vec2(current_cell.x - 1, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x, current_cell.y));
		cells_to_draw_.push_back(glm::vec2(current_cell.x + 1, current_cell.y));
	} else if (current_cell.x > 0 && current_cell.y > 0 &&						// Inner
		current_cell.x < grid_column && current_cell.y < grid_row) {					// 1, 1 ,1
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