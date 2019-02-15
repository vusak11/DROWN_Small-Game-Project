#include "render.h"

void Render::DrawScene() {
}

Render::Render() {
	quad_vertex_array_object_ = 0;
	quad_vertex_buffer_object_ = 0;
	nr_of_lights_ = 1;

	geometry_pass_ = new ShaderHandler(
		"glsl/geometrypass/geometry_vs.glsl",
		"glsl/geometrypass/geometry_gs.glsl",
		"glsl/geometrypass/geometry_fs.glsl");
	lighting_pass_ = new ShaderHandler(
		"glsl/lightingpass/lighting_vs.glsl",
		"glsl/lightingpass/lighting_fs.glsl");
	lights_ = new Light[nr_of_lights_];

	model_ = new Model*[nr_of_models_];
	model_[0] = new Model((char*)"../Resources/Models/TestBox/testBOX.obj");

	/*map_handler_.InitializeMaps(
		"../Resources/Map/TestMap_Sliced/TestMap_",
		"../Resources/Map/rock.png");*/

	map_handler_.InitializeMaps(
		"../Resources/Map/TestMap.bmp",
		"../Resources/Map/rock.png");
}

Render::~Render() {
	delete geometry_pass_;
	delete lighting_pass_;
	delete[] lights_;

	for (int i = 0; i < nr_of_models_; i++) {
		delete model_[i];
	}
	delete[] model_;
}

void Render::InitializeRender() {
	geometry_pass_->GeometryFrameBuffers();

	lights_[0].LightDefault(
		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	map_handler_.InitializeBuffers(geometry_pass_->GetProgram());
	//map_[0].Buffer(geometry_pass_->GetProgram());
}

void Render::UpdateRender(
	float dt, 
	glm::vec3 camera_position,
	glm::mat4 perspective_view_matrix,
	std::vector<ObjectPackage>& object_vector) {

	// Pushing Map into object vector
	glm::mat4 map_matrix = glm::mat4(1.0f);
	map_matrix = glm::translate(map_matrix, glm::vec3(-100.0f, 100.0, -100.0f));
	//map_matrix = glm::rotate(map_matrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
	map_matrix = glm::scale(map_matrix, glm::vec3(1.0f, 1.0f, 0.1f));
	ObjectPackage map_package;
	map_package.id = OBJECT_ID_MAP;
	map_package.model_matrix = map_matrix;

	object_vector.push_back(map_package);

	//  GEOMETRY
	GeometryPass(camera_position, perspective_view_matrix);
	GeometryDrawing(object_vector);
	
	//  LIGHTING
	lights_[0].SetPos(camera_position);
	LightingPass(camera_position);

	RenderQuad();
}

void Render::GeometryDrawing(std::vector<ObjectPackage>& object_vector) {
	/*for (unsigned int i = 0; i < object_vector.size(); i++) {
		if (OBJECT_ID_NULL == object_vector[i].id) {

		}
		else if (OBJECT_ID_PLAYER == object_vector[i].id) {
			ModelTransformation(object_vector[i].model_matrix);
			model_[0]->Draw(geometry_pass_->GetProgram());
		}
		else if (OBJECT_ID_JOHNNY_BRAVO == object_vector[i].id) {
			ModelTransformation(object_vector[i].model_matrix);
		}
		else if (OBJECT_ID_MAP == object_vector[i].id) {
			ModelTransformation(object_vector[i].model_matrix);
			map_handler_.Draw(geometry_pass_->GetProgram());
		}
	}*/
	/*for (int j = 0; j < map_handler_.GetHeightSize(); j++) {
		for (int i = 0; i < map_handler_.GetWidthSize(j); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, map_handler_.Transformation(i, j));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.1f));
			ModelTransformation(model);
			map_handler_.Draw(geometry_pass_->GetProgram(), i, j);
		}
	}*/
}

void Render::ModelTransformation(glm::mat4 model_matrix) {
	glUniformMatrix4fv(glGetUniformLocation(geometry_pass_->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
}

void Render::GeometryPass(
	glm::vec3 camera_position,
	glm::mat4 perspective_view_matrix) {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, geometry_pass_->GetBuffer());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	geometry_pass_->Use();

	glUniform3fv(glGetUniformLocation(
		geometry_pass_->GetProgram(), "view_position"), 1, glm::value_ptr(camera_position)); //OBS camera position
	glUniformMatrix4fv(glGetUniformLocation(
		geometry_pass_->GetProgram(), "projection_view"), 1, GL_FALSE, glm::value_ptr(perspective_view_matrix)); //OBS view
}

void Render::LightingPass(glm::vec3 camera_position) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lighting_pass_->Use();

	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_position"), 0);
	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_normal"), 1);
	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_albedo_spec"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, geometry_pass_->GetPosition());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, geometry_pass_->GetNormal());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, geometry_pass_->GetAlbedoSpecular());

	glUniform3f(
		glGetUniformLocation(lighting_pass_->GetProgram(), "light_position"),
		camera_position.x, camera_position.y, camera_position.z
	);
	glUniform3fv(glGetUniformLocation(lighting_pass_->GetProgram(), "viewPos"), 1, glm::value_ptr(camera_position)); //Add camera positions

}

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
void Render::RenderQuad() {
	if (quad_vertex_array_object_ == 0)
	{
		GLfloat quad_vertices[] = {
			// Positions        // Texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f, //MORE PASTA
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quad_vertex_array_object_);
		glGenBuffers(1, &quad_vertex_buffer_object_);
		glBindVertexArray(quad_vertex_array_object_);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_buffer_object_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		
	}
	glBindVertexArray(quad_vertex_array_object_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
	glBindVertexArray(0);
}