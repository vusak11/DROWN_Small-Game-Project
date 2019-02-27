#include "map.h"

//GLEW
#include <GL/glew.h>
#include <Soil\SOIL.h>

Map::Map() {
}

Map::Map(char* path) {
	if (!LoadMap(path)) {
		std::cout << "Error load height map" << std::endl;
	}
}

Map::~Map() {
}

bool Map::LoadMap(const char* path) {
	unsigned char* map_data = SOIL_load_image(path, &image_width_, &image_height_, 0, SOIL_LOAD_RGB);

	std::cout << "MAP: " << path << SOIL_last_result() << std::endl;
	std::vector<float> temp_float;

	//-------------Height-------------//
	for (int z = 0; z < image_height_; z++) {
		temp_float.clear();
		for (int x = 0; x < image_width_; x++) {
			// Read every third value
			unsigned char color = map_data[3 * (z * image_width_ + x)];
			float height = color;
			temp_float.push_back(height);
		}
		height_map_.push_back(temp_float);
	}
	// No error checks
	return true;
}

void Map::CreateCells(int grid_column, int grid_row, int grid_y, int grid_x) {
	cell_vertices_.clear();
	float cell_width = image_width_ / grid_column;
	float cell_height = image_height_ / grid_row;

	// Create cells and add extra vertices depending on what cell for filling gaps between cells
	if (grid_y == 0 && grid_x == 0) // Upper left corner
		CalculateBorders(0, 2, 0, 2, cell_width, cell_height, grid_y, grid_x);

	if (grid_y == (grid_row - 1) && grid_x == 0) // Lower left corner
		CalculateBorders(2, 0, 0, 2, cell_width, cell_height, grid_y, grid_x);
	
	if (grid_y == 0 && grid_x == (grid_column - 1)) // Upper right corner
		CalculateBorders(0, 2, 2, 0, cell_width, cell_height, grid_y, grid_x);
	
	if (grid_y == (grid_row - 1) && grid_x == (grid_column - 1)) // Lower corner right
		CalculateBorders(2, 0, 2, 0, cell_width, cell_height, grid_y, grid_x);
	
	if (grid_y == 0 && grid_x > 0 && grid_x < grid_column - 1) // Upper side
		CalculateBorders(0, 2, 1, 1, cell_width, cell_height, grid_y, grid_x);
	
	if (grid_y > 0 && grid_y < grid_row - 1 && grid_x == 0) // Left side
		CalculateBorders(1, 1, 0, 2, cell_width, cell_height, grid_y, grid_x);
	
	if (grid_y > 0 && grid_y < grid_row - 1 && grid_x == (grid_column - 1)) // Right side
		CalculateBorders(1, 1, 2, 0, cell_width, cell_height, grid_y, grid_x);
	
	if (grid_y == (grid_row - 1) && grid_x > 0 && grid_x < grid_column - 1) // Lower side
		CalculateBorders(2, 0, 1, 1, cell_width, cell_height, grid_y, grid_x);
	
	if (grid_y != 0 && grid_x != 0 && grid_y != grid_row - 1 && grid_x != grid_column - 1) // Inner
		CalculateBorders(1, 1, 1, 1, cell_width, cell_height, grid_y, grid_x);

	cell_width_ = cell_vertices_[grid_y].size();
	cell_height_ = cell_vertices_.size();
}

void Map::CalculateBorders(
	int b_1, int b_2, int b_3, int b_4,
	float cell_width, float cell_height,
	int grid_y, int grid_x) {

	std::vector<float> temp_cell_width;

	for (int y = (cell_height * grid_y) - b_1; y < (cell_height * (grid_y + 1)) + b_2; y++) {
		temp_cell_width.clear();
		for (int x = (cell_width * grid_x) - b_3; x < (cell_width * (grid_x + 1)) + b_4; x++) {
			temp_cell_width.push_back(height_map_[y][x]);
		}
		cell_vertices_.push_back(temp_cell_width);
	}
}

void Map::UVCoordinates() {
	std::vector<glm::vec2> temp_coord;
	tex_coordinates_.clear();
	for (int z = 0; z < cell_height_; z++) {
		temp_coord.clear();
		for (int x = 0; x < cell_width_; x++) {
			float f_scale_c = (float(x) / float(cell_width_ - 1));
			float f_scale_r = (float(z) / float(cell_height_ - 1));
			temp_coord.push_back(glm::vec2(f_scale_c, f_scale_r));
		}
		tex_coordinates_.push_back(temp_coord);
	}
}

void Map::CalculateNormals() {
	// Map out points on each cell. triangles out of the points
	// Calculate normals when we have the triangles
	// On certain points we sum up all neighbouring normals
	glm::vec3 point_1, point_2, point_3, point_4;
	glm::vec3 edge_1, edge_2;
	std::vector<glm::vec3> ancillary_normal_0, ancillary_normal_1;
	std::vector<std::vector<glm::vec3>> triangle_normal_0, triangle_normal_1;

	for (int z = 0; z < cell_height_ - 1; z++) {
		ancillary_normal_0.clear();
		ancillary_normal_1.clear();
		for (int x = 0; x < cell_width_ - 1; x++) {

			point_1 = glm::vec3((float)x, cell_vertices_[z][x], (float)z);
			point_2 = glm::vec3((float)x, cell_vertices_[z + 1][x], (float)z + 1);
			point_3 = glm::vec3((float)x + 1, cell_vertices_[z + 1][x + 1], (float)z + 1);
			point_4 = glm::vec3((float)x + 1, cell_vertices_[z][x + 1], float(z));

			//---------Triangle 1-------------//
			edge_1 = glm::vec3(point_2 - point_1);
			edge_2 = glm::vec3(point_3 - point_1);

			glm::vec3 normal_0 = glm::normalize(glm::cross(edge_1, edge_2));

			//--------Triangle 2-------------//
			edge_1 = glm::vec3(point_1 - point_4);
			edge_2 = glm::vec3(point_3 - point_4);

			glm::vec3 normal_1 = glm::normalize(glm::cross(edge_1, edge_2));

			ancillary_normal_0.push_back(normal_0);
			ancillary_normal_1.push_back(normal_1);
		}
		triangle_normal_0.push_back(ancillary_normal_0);
		triangle_normal_1.push_back(ancillary_normal_1);
	}

	//---------------Sum Normals(adjacent)------------//
	normals_.clear();
	std::vector<glm::vec3> temp_sum;
	for (int z = 0; z < cell_height_; z++) {
		temp_sum.clear();
		for (int x = 0; x < cell_width_; x++) {
			glm::vec3 sum_normal = glm::vec3(0.0f, 0.0f, 0.0f);
			if (z != 0 && x != 0) {
				sum_normal += triangle_normal_0[z - 1][x - 1];
				sum_normal += triangle_normal_1[z - 1][x - 1];
			}
			if (z != 0 && x != cell_width_ - 1) {
				sum_normal += triangle_normal_0[z - 1][x];
			}
			if (z != cell_height_ - 1 && x != cell_width_ - 1) {
				sum_normal += triangle_normal_0[z][x];
				sum_normal += triangle_normal_1[z][x];
			}
			if (z != cell_height_ - 1 && x != 0) {
				sum_normal += triangle_normal_1[z][x - 1];
			}
			sum_normal = glm::normalize(sum_normal);
			temp_sum.push_back(sum_normal);
		}
		normals_.push_back(temp_sum);
	}
}

void Map::CreateTriangles() {
	Triangle triangle;
	vertices_.clear();
	int index = 0;
	for (int z = 0; z < cell_height_; z++) {
		for (int x = 0; x < cell_width_; x++) {
			triangle.x = (float)x;
			triangle.y = (float)z * -1;
			triangle.z = cell_vertices_[z][x];
			triangle.u = tex_coordinates_[z][x].x;
			triangle.v = tex_coordinates_[z][x].y;
			triangle.x_normal = normals_[z][x].x;
			triangle.y_normal = normals_[z][x].z * -1;
			triangle.z_normal = normals_[z][x].y;
			vertices_.push_back(triangle);
		}
	}
}

void Map::CreateIndices() {
	indices_.clear();

	for (int z = 0; z < cell_height_ - 1; z++) { // -1 so that we don't reach out of bounds
		for (int x = 0; x < cell_width_ - 1; x++) { // -------- || ------------
			int index = (z * cell_width_) + x;

			int p1 = index;
			int p2 = index + cell_width_ + 1;
			int p3 = index + 1;

			int p4 = index;
			int p5 = index + cell_width_;
			int p6 = index + cell_width_ + 1;

			indices_.push_back(p1);
			indices_.push_back(p2);
			indices_.push_back(p3);
			indices_.push_back(p4);
			indices_.push_back(p5);
			indices_.push_back(p6);

		}
		indices_.push_back(cell_width_*cell_height_); //degenerate strips
	}
}

void Map::LoadTexture(const char * texture_name_0, const char* texture_name_1) {
	//Texture 1
	int texWidth, texHeight;
	unsigned char* image = SOIL_load_image(texture_name_0, &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

	std::cout << texture_name_0 << SOIL_last_result() << std::endl;

	glGenTextures(1, &texture0_);
	glBindTexture(GL_TEXTURE_2D, texture0_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, 
				 GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	// Texture 2
	image = SOIL_load_image(texture_name_1, &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

	std::cout << texture_name_1 << SOIL_last_result() << std::endl;

	glGenTextures(1, &texture1_);
	glBindTexture(GL_TEXTURE_2D, texture1_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}

void Map::Buffer(GLuint shader) {
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);

	glGenBuffers(1, &vertex_buffer_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Triangle),
					&vertices_[0], GL_STATIC_DRAW);

	glGenBuffers(1, &element_buffer_object_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * 4, &indices_[0], GL_STATIC_DRAW);

	//Vertex
	glEnableVertexAttribArray(0);
	GLuint vertexPos = glGetAttribLocation(shader, "position");
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle), BUFFER_OFFSET(0));

	//Texture
	glEnableVertexAttribArray(1);
	GLuint texCoords = glGetAttribLocation(shader, "tex_coordinates");
	glVertexAttribPointer(texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle),
					BUFFER_OFFSET(sizeof(float) * 3));

	//Normal
	glEnableVertexAttribArray(2);
	GLuint vertexNormal = glGetAttribLocation(shader, "normal");
	glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle),
					BUFFER_OFFSET(sizeof(float) * 5));

	glBindVertexArray(0);
}

void Map::Draw(GLuint shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0_);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1_);

	glUniform1i(glGetUniformLocation(shader, "texture_diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader, "texture_diffuse1"), 1);

	glBindVertexArray(vertex_array_object_);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(cell_width_ * cell_height_);
	
	glDrawElements(GL_TRIANGLE_STRIP, indices_.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glDisable(GL_PRIMITIVE_RESTART);
}

int Map::GetCellHeight() const {
	return cell_height_;
}
std::vector<std::vector<float>>* Map::GetTempHeightList() {
	return &height_map_;
}

void Map::CleanMapInfo() {
	height_map_.clear();
}

std::vector<std::vector<float>> Map::GetTestList() {
	return height_map_;
}


//int Map::GetHeight() {
//	return map_height_;
//}

int Map::GetCellWidth() const {
	return cell_width_;
}