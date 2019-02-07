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

bool Map::LoadMap(char* path) {

	int map_width_, map_height_;
	unsigned char* map_data = SOIL_load_image(path, &map_width_, &map_height_, 0, SOIL_LOAD_RGB);

	std::cout << "MAP: " << path << SOIL_last_result() << std::endl;

	std::vector<float> temp_float;

	//-------------Height-------------//
	for (int z = 0; z < map_height_; z++) {
		temp_float.clear();
		for (int x = 0; x < map_width_; x++) {
			// Read every third value
			unsigned char color = map_data[3 * (z * map_width_ + x)];
			float height = (map_height_ * (color / 255.0f));
			temp_float.push_back(height);
		}
		temp_height_.push_back(temp_float);
	}

	//lowerPosition();

	//-------------Texture coordinates(U,V)-------------//

	std::vector<glm::vec2> temp_coord;

	for (int z = 0; z < map_height_; z++) {
		temp_coord.clear();
		for (int x = 0; x < map_width_; x++) {
			float f_scale_c = (float(x) / float(map_width_ - 1)) * 10;
			float f_scale_r = (float(z) / float(map_height_ - 1)) * 10;
			temp_coord.push_back(glm::vec2(f_scale_c, f_scale_r));
		}
		tex_coord_.push_back(temp_coord);
	}

	//------------ Normals ------------//

	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 point3;
	glm::vec3 point4;

	glm::vec3 edge1;
	glm::vec3 edge2;

	std::vector<glm::vec3> temp_norm_0;
	std::vector<glm::vec3> temp_norm_1;

	std::vector<std::vector<glm::vec3>> triangle_norm_0;
	std::vector<std::vector<glm::vec3>> triangle_norm_1;

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
		//indices_.push_back(map_width_*map_height_); //"degenerate"
	}

	//-------------Create triangles-------------//
	Triangle temp_triangle;
	int index = 0;

	for (int z = 0; z < map_height_; z++) {
		for (int x = 0; x < map_width_; x++) {
			temp_triangle.x = (float)x;
			temp_triangle.y = temp_height_[z][x];
			temp_triangle.z = (float)z;
			temp_triangle.u = tex_coord_[z][x].x;
			temp_triangle.v = tex_coord_[z][x].y;
			temp_triangle.x_normal = normals_[z][x].x;
			temp_triangle.y_normal = normals_[z][x].y;
			temp_triangle.z_normal = normals_[z][x].z;
			vertices_.push_back(temp_triangle);
		}
	}

	return true;
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
	GLuint vertexPos = glGetAttribLocation(shader, "a_position");
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle), BUFFER_OFFSET(0));

	//Texture
	glEnableVertexAttribArray(1);
	GLuint texCoords = glGetAttribLocation(shader, "a_coordinates");
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

	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexture1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gTexture2);*/

	glUniform1i(glGetUniformLocation(shader, "texture_diffuse"), 0);
	/*glUniform1i(glGetUniformLocation(shader.GetgProgram(), "rock"), 1);
	glUniform1i(glGetUniformLocation(shader.GetgProgram(), "snow"), 2);*/

	glBindVertexArray(vertex_array_object_);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(map_width_ * map_height_);

	glDrawElements(GL_TRIANGLE_STRIP, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Map::SetHeight(int height) {
	map_height_ = height;
}

void Map::SetWidth(int width) {
	map_width_ = width;
}

int Map::GetHeight() {
	return map_height_;
}

int Map::GetWidth() {
	return map_width_;
}
