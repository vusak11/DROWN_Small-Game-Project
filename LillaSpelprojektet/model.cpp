#include "model.h"

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene) {
	std::vector<Vertex> vertex;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	//Process the mesh
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex temp_vertex;
		glm::vec3 vector_vertices;
		//Process vertices
		vector_vertices.x = mesh->mVertices[i].x;
		vector_vertices.y = mesh->mVertices[i].y;
		vector_vertices.z = mesh->mVertices[i].z;
		temp_vertex.position = vector_vertices;
		//Process textures
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vector_textures;
			vector_textures.x = mesh->mTextureCoords[0][i].x;
			vector_textures.y = mesh->mTextureCoords[0][i].y;
			temp_vertex.texture = vector_textures;
		}
		else {
			temp_vertex.texture = glm::vec2(0.0f, 0.0f);
		}
		//Process normals
		if (mesh->mNormals) {
			glm::vec3 vector_normals;
			vector_normals.x = mesh->mNormals[i].x;
			vector_normals.y = mesh->mNormals[i].y;
			vector_normals.z = mesh->mNormals[i].z;
			temp_vertex.normals = vector_normals;
		}
		vertex.push_back(temp_vertex);
	}
	
	//Process faces/indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace faces = mesh->mFaces[i];
		for (GLuint y = 0; y < faces.mNumIndices; y++) {
			indices.push_back(faces.mIndices[y]);
		}
	}

	//Process materials
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = LoadMaterialTextures(
			material, 
			aiTextureType_DIFFUSE, 
			"texture_diffuse" //Make sure this in GLSL
		);
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<Texture> specular_maps = LoadMaterialTextures(
			material,
			aiTextureType_SPECULAR,
			"texture_specular" //Make sure this in GLSL
		);
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}

	return Mesh(vertex, indices, textures);
}

GLint Model::TextureFromFile(const char * path, std::string directory) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	//Generate texture id
	GLuint texture_id;
	glGenTextures(1, &texture_id);

	sf::Image image;
	image.loadFromFile(filename);

	//Generate load texture data
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA, 
		image.getSize().x,
		image.getSize().y,
		0,
		GL_RGBA,
		GL_UNSIGNED_INT,
		image.getPixelsPtr()
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Set some parameters for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); //Unbind the texture

	return texture_id;
}

std::vector<Texture> Model::LoadMaterialTextures(
	aiMaterial * material, 
	aiTextureType type,
	std::string type_name) {
	std::vector<Texture> texture;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString ai_string;
		material->GetTexture(type, i, &ai_string);

		bool skip_loop = false;
		//Check if the texture already been loaded
		for (unsigned int y = 0; y < texture_loaded_.size() && !skip_loop; y++) {
			if (strcmp(texture_loaded_[y].path.C_Str(), ai_string.C_Str()) == 0) {
				texture.push_back(texture_loaded_[y]);
				skip_loop = true;
			}
		}
		if (!skip_loop) {
			Texture temp_texture;
			temp_texture.id_texture = TextureFromFile(ai_string.C_Str(), directory_);
			temp_texture.type = type_name;
			temp_texture.path = ai_string;
			
			texture.push_back(temp_texture);
			texture_loaded_.push_back(temp_texture);
		}
	}

	return texture;
}

void Model::LoadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(
		path, 
		aiProcess_Triangulate | aiProcess_FlipUVs
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ASSIMP LOAD MODEL ERROR: " << import.GetErrorString() << std::endl;
		return;
	}

	directory_ = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene) {
	//Process all the nodes meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mesh_.push_back(ProcessMesh(mesh, scene));
	}
	//Then process nodes children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Model::Model() {
}

Model::~Model() {
}

void Model::LoadModelFromFile(GLchar * path) {
	LoadModel(path);
}

void Model::Draw(GLuint shader) {
	for (unsigned int i = 0; i < mesh_.size(); i++) {
		mesh_[i].Draw(shader);
	}
}
