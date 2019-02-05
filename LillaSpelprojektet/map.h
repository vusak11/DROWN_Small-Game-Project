#ifndef MAP_H
#define MAP_H

//#include <GLM/vec2.hpp>
//#include <GLM/vec3.hpp>
//#include <GLM/vec4.hpp>
#include <GLM/glm.hpp>

#include "shader_handler.h"

// This class is symbolising the a map in the game and all its proporties.  
// Example:
//    map->GetMapWidth();
//  

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
	~Map();

	// This function is not complete.
	bool LoadMap(char* texture_name);	// Returns a bool to trouble shoot while loading new maps.
	void LoadTexture(char* texture_name);

	void Buffer(GLuint shader);
	void Draw(GLuint shader);

	void SetHeight(int height);
	void SetWidth(int width);

	float GetTempHeight(int x, int z);
	void LowerPosition();

	int GetHeight();
	int GetWidth();
private:
	int map_width_;			// Width of map image
	int map_height_;		// Height of map image

	std::vector<Triangle> vertices_;
	std::vector<std::vector<float>> temp_height_;
	std::vector<std::vector<glm::vec2>> tex_coord_;
	std::vector<std::vector<glm::vec3>> normals_;
	std::vector<int> indices_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
	GLuint element_buffer_object_;

	GLuint texture_;

};

#endif