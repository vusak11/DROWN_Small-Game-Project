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

//-------------------Method 1
/*bool Map::LoadMap(const char* path) {

	unsigned char* map_data = SOIL_load_image(path, &map_width_, &map_height_, 0, SOIL_LOAD_RGB);

	std::cout << "MAP: " << path << SOIL_last_result() << std::endl;
	std::vector<float> temp_float;

	//-------------Height-------------//
	for (int z = 0; z < map_height_; z++) {
		temp_float.clear();
		for (int x = 0; x < map_width_; x++) {
			// Read every third value
			unsigned char color = map_data[3 * (z * map_width_ + x)];
			float height = color;
			temp_float.push_back(height);
		}
		temp_height_.push_back(temp_float);
	}

	//-------------Texture coordinates(U,V)-------------//

	std::vector<glm::vec2> temp_coord;

	for (int z = 0; z < temp_height_.size(); z++) {
		temp_coord.clear();
		for (int x = 0; x < map_width_; x++) {
			float f_scale_c = (float(x) / float(map_width_ - 1)) * 10;
			float f_scale_r = (float(z) / float(temp_height_.size() - 1)) * 10;
			temp_coord.push_back(glm::vec2(f_scale_c, f_scale_r));
		}
		tex_coord_.push_back(temp_coord);
	}

	//------------ Normals ------------//

	glm::vec3 point1, point2, point3, point4;
	glm::vec3 edge1, edge2;
	std::vector<glm::vec3> temp_norm_0, temp_norm_1;
	std::vector<std::vector<glm::vec3>> triangle_norm_0, triangle_norm_1;

	for (int z = 0; z < map_height_ - 1; z++) {
		temp_norm_0.clear();
		temp_norm_1.clear();
		for (int x = 0; x < map_width_ - 1; x++) {

			point1 = glm::vec3((float)x, temp_height_[z][x], (float)z);
			point2 = glm::vec3((float)x, temp_height_[z + 1][x], (float)z + 1);
			point3 = glm::vec3((float)x + 1, temp_height_[z + 1][x + 1], (float)z + 1);
			point4 = glm::vec3((float)x + 1, temp_height_[z][x + 1], float(z));

			//---------Triangle 1-------------//
			edge1 = glm::vec3(point2 - point1);
			edge2 = glm::vec3(point3 - point1);

			glm::vec3 normal0 = glm::normalize(glm::cross(edge1, edge2));

			//--------Triangle 2-------------//
			edge1 = glm::vec3(point1 - point4);
			edge2 = glm::vec3(point3 - point4);

			glm::vec3 normal1 = glm::normalize(glm::cross(edge1, edge2));

			temp_norm_0.push_back(normal0);
			temp_norm_1.push_back(normal1);
		}
		triangle_norm_0.push_back(temp_norm_0);
		triangle_norm_1.push_back(temp_norm_1);
	}

	//---------------Sum Normals(adjacent)------------//

	std::vector<glm::vec3> temp_sum;
	for (int z = 0; z < map_height_; z++) {
		temp_sum.clear();
		for (int x = 0; x < map_width_; x++) {
			glm::vec3 sum_normal = glm::vec3(0.0f, 0.0f, 0.0f);
			if (z != 0 && x != 0) {
				sum_normal += triangle_norm_0[z - 1][x - 1];
				sum_normal += triangle_norm_1[z - 1][x - 1];
			}
			if (z != 0 && x != map_width_ - 1) {
				sum_normal += triangle_norm_0[z - 1][x];
			}
			if (z != map_height_ - 1 && x != map_width_ - 1) {
				sum_normal += triangle_norm_0[z][x];
				sum_normal += triangle_norm_1[z][x];
			}
			if (z != map_height_ - 1 && x != 0) {
				sum_normal += triangle_norm_1[z][x - 1];
			}
			sum_normal = glm::normalize(sum_normal);
			temp_sum.push_back(sum_normal);
		}
		normals_.push_back(temp_sum);
	}

	for (int z = 0; z < map_height_ - 1; z++) {
		for (int x = 0; x < map_width_ - 1; x++) {
			int index = (z * map_width_) + x;

			int p1 = index;
			int p2 = index + map_width_ + 1;
			int p3 = index + 1;

			int p4 = index;
			int p5 = index + map_width_;
			int p6 = index + map_width_ + 1;

			indices_.push_back(p1);
			indices_.push_back(p2);
			indices_.push_back(p3);
			indices_.push_back(p4);
			indices_.push_back(p5);
			indices_.push_back(p6);

		}
		indices_.push_back(map_width_*map_height_); //"degenerate"
	}

	//-------------Create triangles-------------//
	Triangle temp_triangle;
	int index = 0;

	for (int z = 0; z < map_height_; z++) {
		for (int x = 0; x < map_width_; x++) {
			temp_triangle.x = (float)x;
			temp_triangle.y = (float)z * -1;
			temp_triangle.z = temp_height_[z][x];
			temp_triangle.u = tex_coord_[z][x].x;
			temp_triangle.v = tex_coord_[z][x].y;
			temp_triangle.x_normal = normals_[z][x].x;
			temp_triangle.y_normal = normals_[z][x].z * -1;
			temp_triangle.z_normal = normals_[z][x].y;
			vertices_.push_back(temp_triangle);
		}
	}

	return true;
}
*/

//-------------------- Method 2
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
}

void Map::CreateCells(unsigned int grid_column, unsigned int grid_row, int j, int i) {
	cell_vertices_.clear();
	float cell_width = image_width_ / grid_column;
	float cell_height = image_height_ / grid_row;

	std::vector<float> temp_cell_width;
	std::vector<std::vector<std::vector<float>>> temp_cell_storage;

	if (j == 0 && i == 0) { // Upper left corner
		for (int y = (cell_height * j) - 0; y < (cell_height * (j + 1)) + 2; y++) {
			temp_cell_width.clear();
			for (int x = (cell_width * i) - 0; x < (cell_width * (i + 1)) + 2; x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}
	
	if (j == (grid_row - 1) && i == 0) { // Down left corner
		for (int y = (cell_height * j) - 2; y < (cell_height * (j + 1)); y++) {
			temp_cell_width.clear();
			for (int x = (cell_width * i) - 0; x < (cell_width * (i + 1)) + 2; x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}
	
	if (j == 0 && i == (grid_column - 1)) { // Upper right corner
		for (int y = (cell_height * j) - 0; y < (cell_height * (j + 1)) + 2; y++) {
			temp_cell_width.clear();
			for (int x = (cell_width * i) - 2; x < (cell_width * (i + 1)); x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}
	
	if (j == (grid_row - 1) && i == (grid_column - 1)) { // Down corner right
		for (int y = (cell_height * j) - 2; y < (cell_height * (j + 1)); y++) {
			temp_cell_width.clear();
			for (int x = (cell_width * i) - 2; x < (cell_width * (i + 1)); x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}
	
	if (j == 0 && i > 0 && i < grid_column - 1) { // Upper side
		for (int y = (cell_height * j) - 0; y < (cell_height * (j + 1)) + 2; y++) {
			temp_cell_width.clear();
			for (int x = (cell_width * i) - 1; x < (cell_width * (i + 1)) + 1; x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}
	
	if (j > 0 && j < grid_row - 1 && i == 0) { // Left side
		for (int y = (cell_height * j) - 1; y < (cell_height * (j + 1)) + 1; y++) {
			temp_cell_width.clear();
			for (int x = (cell_width * i) - 0; x < (cell_width * (i + 1)) + 2; x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}
	
	if (j > 0 && j < grid_row - 1 && i == (grid_column - 1)) { // Right side
		for (int y = (cell_height * j) - 1; y < (cell_height * (j + 1)) + 1; y++) {
			temp_cell_width.clear();
			for (int x = (cell_width * i) - 2; x < (cell_width * (i + 1)); x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}
	
	if (j == (grid_row - 1) && i > 0 && i < grid_column - 1) { // Down side
		for (int y = (cell_height * j) - 2; y < (cell_height * (j + 1)); y++) {
			temp_cell_width.clear();
			for (int x = (cell_width * i) - 1; x < (cell_width * (i + 1)) + 1; x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}
	
	if (j != 0 && i != 0 && j != grid_row - 1 && i != grid_column - 1) { // Inner
		for (int y = -1 + (cell_height * j); y < (cell_height * (j + 1)) + 1; y++) {
			temp_cell_width.clear();
			for (int x = -1 + (cell_width * i); x < (cell_width * (i + 1)) + 1; x++) {
				temp_cell_width.push_back(height_map_[y][x]);
			}
			cell_vertices_.push_back(temp_cell_width);
		}
	}

	cell_width_ = cell_vertices_[j].size();
	cell_height_ = cell_vertices_.size();
}

void Map::UVCoordinates() {
	std::vector<glm::vec2> temp_coord;

	for (int z = 0; z < cell_height_; z++) {
		temp_coord.clear();
		for (int x = 0; x < cell_width_; x++) {
			float f_scale_c = (float(x) / float(cell_width_ - 1)) * 10;
			float f_scale_r = (float(z) / float(cell_height_ - 1)) * 10;
			temp_coord.push_back(glm::vec2(f_scale_c, f_scale_r));
		}
		tex_coordinates_.push_back(temp_coord);
	}
}

void Map::CalculateNormals() {
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
	Triangle temp_triangle;
	vertices_.clear();
	int index = 0;
	for (int z = 0; z < cell_height_; z++) {
		for (int x = 0; x < cell_width_; x++) {
			temp_triangle.x = (float)x;
			temp_triangle.y = (float)z * -1;
			temp_triangle.z = cell_vertices_[z][x];
			temp_triangle.u = tex_coordinates_[z][x].x;
			temp_triangle.v = tex_coordinates_[z][x].y;
			temp_triangle.x_normal = normals_[z][x].x;
			temp_triangle.y_normal = normals_[z][x].z * -1;
			temp_triangle.z_normal = normals_[z][x].y;
			vertices_.push_back(temp_triangle);
		}
	}
}

void Map::CreateIndices() {
	indices_.clear();

	for (int z = 0; z < cell_height_ - 1; z++) {
		for (int x = 0; x < cell_width_ - 1; x++) {
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
		indices_.push_back(cell_width_*cell_height_); //"degenerate"
	}
}

void Map::LoadTexture(char * texture_name) {
	int texWidth, texHeight;
	unsigned char* image = SOIL_load_image(texture_name, &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

	std::cout << texture_name << SOIL_last_result() << std::endl;

	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

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
	glBindTexture(GL_TEXTURE_2D, texture_);

	glUniform1i(glGetUniformLocation(shader, "texture_diffuse"), 0);

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

int Map::GetCellWidth() const {
	return cell_width_;
}

int Map::GetImageHeight() {
	return image_height_;
}

int Map::GetImageWidth() {
	return image_width_;
}
