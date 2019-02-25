#ifndef MESH_H
#define MESH_H

#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <ASSIMP/scene.h>
#include <vector>

#include "../../Core/shader_handler.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 texture;
};

struct Texture {
	GLuint id_texture;
	std::string type;
	aiString path;
};

class Mesh {
private:
	GLuint vertex_array_object_, vertex_buffer_object_, element_buffer_object_;

	std::vector<Vertex> vertices_;
	std::vector<GLuint> indices_;
	std::vector<Texture> textures_;

	void SetupMesh();
public:
	Mesh(
		std::vector<Vertex> vertices, 
		std::vector<GLuint> indices, 
		std::vector<Texture> textures
	);
	~Mesh();

	void Draw(GLuint shader);
};

#endif

