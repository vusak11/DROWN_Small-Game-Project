#include "map_handler.h"



MapHandler::MapHandler(){
	grid_column_ = 5;
	grid_row_ = 5;
}

MapHandler::~MapHandler(){
	//delete cells_to_draw_;
}

void MapHandler::InitializeMaps(std::string map_path, char* texture_path) {
	float width, height = 0;

	Map ancillary_map;
	ancillary_map.LoadMap(map_path.c_str());
	ancillary_map.LoadTexture(texture_path);

	GridMap grid_cell;
	std::vector<GridMap> grid_cells;

	for (int j = 0; j < grid_row_; j++) {
		grid_cells.clear();
		width = 0;
		for (int i = 0; i < grid_column_; i++) {
			ancillary_map.CreateCells(grid_column_, grid_row_, j, i);
			ancillary_map.UVCoordinates();
			ancillary_map.CalculateNormals();
			ancillary_map.CreateTriangles();
			ancillary_map.CreateIndices();

			grid_cell.map_cell_ = ancillary_map;
			grid_cell.translate_.x = width;
			grid_cell.translate_.y = height;
			grid_cell.translate_.z = 0.0f;
			grid_cell.cell_position_ = glm::vec2(i, j);
			grid_cells.push_back(grid_cell);

			width += grid_cell.map_cell_.GetCellWidth() - 2.0f; // -2.0f is for closing gap
		}
		height -= grid_cell.map_cell_.GetCellHeight() - 2.0f; // --------- || ----------
		grid_map_.push_back(grid_cells);
	}
}

void MapHandler::InitializeBuffers(GLuint shader) {
	for (int j = 0; j < grid_map_.size(); j++) {
		for (int i = 0; i < grid_map_.size(); i++) {
			grid_map_[j][i].map_cell_.Buffer(shader);
		}
	}
}

glm::vec3 MapHandler::Transformation(unsigned int i, unsigned int j) {
	return grid_map_[j][i].translate_;
}

void MapHandler::Draw(GLuint shader, unsigned int i, unsigned int j) {
	grid_map_[j][i].map_cell_.Draw(shader);
}


glm::vec2 MapHandler::CurrentCell(glm::vec3 players_current_position) {
	float x, y = 0;
	for (int j = 0; j < grid_map_.size(); j++) {
		for (int i = 0; i < grid_map_[j].size(); i++) {
			if (players_current_position.x > grid_map_[j][i].translate_.x)
				x = i;
			if (players_current_position.y < grid_map_[j][i].translate_.y)
				y = j;
		}
	}

	return glm::vec2(x, y);
}

std::vector<int> MapHandler::GridCulling(glm::vec2 current_cell) {
	cells_to_draw_.clear();
	if (current_cell.x == 0 && current_cell.y == 0) {// Upper left corner
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);

		/*cells_to_draw_ = new int[9] {
			0, 0, 0,
			0, 1, 1,
			0, 1, 1
		};*/
	}

	if (current_cell.x == (grid_column_ - 1) && current_cell.y == 0) {// Upper right corner
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);

		/*cells_to_draw_ = new int[9] {
			0, 0, 0,
			1, 1, 0,
			1, 1, 0
		};*/
	}

	if (current_cell.x == 0 && current_cell.y == (grid_row_ - 1)) {// Down left corner
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);

		/*cells_to_draw_ = new int[9] {
			0, 1, 1,
			0, 1, 1,
			0, 0, 0
		};*/
	}

	if (current_cell.x == (grid_column_ - 1) && current_cell.y == (grid_row_ - 1)) {// Down right corner
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);

		/*cells_to_draw_ = new int[9] {
			1, 1, 0,
			1, 1, 0,
			0, 0, 0
		};*/
	}

	if (current_cell.x > 0 && current_cell.x < grid_column_ && // Upper side
		current_cell.y == 0) {
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);

		/*cells_to_draw_ = new int[9] {
			0, 0, 0,
			1, 1, 1,
			1, 1, 1
		};*/
	}
	if (current_cell.x == 0 && current_cell.y > 0 && current_cell.y < grid_row_) {// Left side
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);


		/*cells_to_draw_ = new int[9] {
			0, 1, 1,
			0, 1, 1,
			0, 1, 1
		};*/
	}

	if (current_cell.x == (grid_column_ - 1) && // Right side
		current_cell.y > 0 && current_cell.y < grid_row_) {
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);

		/*cells_to_draw_ = new int[9]{
			1, 1, 0,
			1, 1, 0,
			1, 1, 0
		};*/
	}

	if (current_cell.x > 0 && current_cell.x < grid_column_ && // Down side
		current_cell.y == (grid_row_ - 1)) {
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);

		/*cells_to_draw_ = new int[9]{
			1, 1, 1,
			1, 1, 1,
			0, 0, 0
		};*/
	}
	if (current_cell.x > 0 && current_cell.y > 0 &&
		current_cell.x < grid_column_ && current_cell.y < grid_row_) {
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(0);
		cells_to_draw_.push_back(1);
		cells_to_draw_.push_back(1);

		/*cells_to_draw_ = new int[9] {
			1, 1, 1,
			1, 1, 1,
			1, 1, 1
		};*/
	}
	return cells_to_draw_;
}

int MapHandler::GetHeightSize() const {
	return grid_map_.size();
}

int MapHandler::GetWidthSize(unsigned int i) const {
	return grid_map_[i].size();
}

Map MapHandler::GetMap(int cam_index)
{
	//return maps_[cam_index];
}
