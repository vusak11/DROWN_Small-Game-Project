#include "map_handler.h"



MapHandler::MapHandler(){
	width_ = 0;
	height_ = 0;
	grid_column_ = 5;
	grid_row_ = 5;
}

MapHandler::~MapHandler(){
}

void MapHandler::InitializeMaps(std::string map_path, char* texture_path) {

	Map ancillary_map;
	ancillary_map.LoadMap(map_path.c_str());
	ancillary_map.LoadTexture(texture_path);

	GridMap grid_cell;
	std::vector<GridMap> grid_cells;

	for (int j = 0; j < grid_row_; j++) {
		grid_cells.clear();
		width_ = 0;
		for (int i = 0; i < grid_column_; i++) {
			ancillary_map.CreateCells(grid_column_, grid_row_, j, i);
			ancillary_map.UVCoordinates();
			ancillary_map.CalculateNormals();
			ancillary_map.CreateTriangles();
			ancillary_map.CreateIndices();

			grid_cell.map_cell_ = ancillary_map;

			grid_cell.translate_.x = width_;
			grid_cell.translate_.y = height_;
			grid_cell.translate_.z = 0.0f;
			grid_cells.push_back(grid_cell);

			width_ += grid_cell.map_cell_.GetCellWidth() - 2.0f; // -2.0f is for closing gap
		}
		height_ -= grid_cell.map_cell_.GetCellHeight() - 2.0f; // --------- || ----------
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


void MapHandler::CurrentCell(glm::vec3 current_position) {
	//for (int j = 0; j < )
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
