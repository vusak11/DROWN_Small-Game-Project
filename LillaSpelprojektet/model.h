#ifndef MODEL_H
#define MODEL_H

#include <SFML/Graphics/Image.hpp>
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>
#include "mesh.h"
class Model {
private:
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	GLint TextureFromFile(const char* path, std::string directory);
	std::vector<Texture> LoadMaterialTextures(
		aiMaterial* material, 
		aiTextureType type, 
		std::string type_name
	);

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);

	std::vector<Texture> texture_loaded_;
	std::vector<Mesh> mesh_;
	std::string directory_;
public:
	Model();
	~Model();

	Model(GLchar* path);
	void Draw(GLuint shader);
};

#endif

