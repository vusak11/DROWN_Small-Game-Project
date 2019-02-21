#ifndef MAP_H
#define MAP_H

#include <GLM/glm.hpp>
#include <GL/glew.h>
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"	// To be able to use vector variables in class.
#include <iostream>
#include <vector>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i)) // Offset variable

class Map
{
private:
	struct Triangle {
		float x, y, z;		// Vertex Position
		float u, v;			// UVs
		float x_normal, y_normal, z_normal;	// Normal values
	};

public:
	Map();
	Map(char* path);
	~Map();

	// This function is not complete.
	//bool LoadMap(const char* path);	// Returns a bool to trouble shoot while loading new maps.
	bool LoadMap(const char* path);
	void LoadTexture(const char* texture_name_0, const char* texture_name_1);

	void CreateCells(
		int grid_column, int grid_row,
		int grid_y, int grid_x
	);
	void CalculateBorders(
		int b_1, int b_2, int b_3, int b_4,
		float cell_width, float cell_height,
		int grid_y, int grid_x
	);
	void UVCoordinates();
	void CalculateNormals();
	void CreateTriangles();
	void CreateIndices();

	void Buffer(GLuint shader);
	void Draw(GLuint shader);

	int GetCellHeight() const;
	int GetCellWidth() const;
	void SetHeight(int height);
	void SetWidth(int width);

	std::vector<std::vector<float>> GetTestList();
	std::vector<std::vector<float>>* GetTempHeightList();	// Used to pass the map height data to physics

private:
		int image_width_;			// Width of map image
		int image_height_;		// Height of map image

	std::vector<std::vector<float>> cell_vertices_;
	float cell_width_;
	float cell_height_;
	
	std::vector<Triangle> vertices_;
	std::vector<std::vector<float>> height_map_;
	std::vector<std::vector<glm::vec2>> tex_coordinates_;
	std::vector<std::vector<glm::vec3>> normals_;
	std::vector<int> indices_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
	GLuint element_buffer_object_;

	GLuint texture0_;
	GLuint texture1_;
};

#endif