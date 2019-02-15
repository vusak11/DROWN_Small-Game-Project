#include "map_handler.h"



MapHandler::MapHandler(){
	grid_column_ = 5;
	grid_row_ = 5;
}

MapHandler::~MapHandler(){
}

//-----------------Method 1
/*void MapHandler::InitializeMaps(std::string map_path, std::string texture_path) {
	bool check = true;
	unsigned int quantity = 0;
	std::vector<GridMap> grid_maps;

	//Detecting how many existing map cells
	while (check) {
		std::ifstream map_file(map_path + std::to_string(++quantity) + ".bmp");
		if (!(map_file.is_open())) {
			check = false;
		}
	}
	unsigned int counter = 0;
	float width = 0;
	std::vector<float> height;
	for (int i = 0; i < sqrt(quantity - 1); i++) {
		height.push_back(0);
	}
	//Create a map grid and load in existing map cells
	for (int j = 0; j < sqrt(quantity - 1); j++) {
		grid_maps.clear();
		width = 0;
		for (int i = 0; i < sqrt(quantity - 1); i++) {
			counter++;
			GridMap grid_map;
			Map cell;

			//Load cells and texture
			cell.LoadMap((map_path + std::to_string(counter) + ".bmp").c_str());
			cell.LoadTexture((char*)"../Resources/Map/rock.png");

			//Store values
			grid_map.map_cell_ = cell;
			grid_map.translate.x = width, height[i], 0.0f;
			grid_map.translate.y = height[i];
			grid_map.translate.z = 0.0f;
			grid_maps.push_back(grid_map);

			//Calculate translation for each cells
			width += cell.GetWidth(); // -1.0f is for closing gap
			height.at(i) -= cell.GetHeight(); //------||-------
		}
		grid_map_.push_back(grid_maps);
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
	return grid_map_[j][i].translate;
}*/

//------------- Method 2
void MapHandler::InitializeMaps(std::string map_path, std::string texture_path) {

	Map ancillary_map;
	ancillary_map.LoadMap(map_path.c_str());

	GridMap grid_cell;
	std::vector<GridMap> grid_cells;

	for (int j = 0; j < grid_row_; j++) {
		grid_cells.clear();
		for (int i = 0; i < grid_column_; i++) {
			ancillary_map.CreateCells(grid_column_, grid_row_, j, i);
			ancillary_map.UVCoordinates();
			ancillary_map.CalculateNormals(); //FIX
			//ancillary_map.CreateTriangles(); //FIX
			grid_cell.map_cell_ = ancillary_map;
			grid_cells.push_back(grid_cell);
		}
		grid_map_.push_back(grid_cells);
	}
	std::cout << "hej" << std::endl;
}

void MapHandler::InitializeBuffers(GLuint shader) {
	for (int j = 0; j < grid_map_.size(); j++) {
		for (int i = 0; i < grid_map_.size(); i++) {
			grid_map_[j][i].map_cell_.Buffer(shader);
		}
	}
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
