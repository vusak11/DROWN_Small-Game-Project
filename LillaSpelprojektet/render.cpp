#include "render.h"

void Render::DrawScene() {
}

Render::Render() {
	quad_vertex_array_object_ = 0;
	quad_vertex_buffer_object_ = 0;
	nr_of_lights_ = 1;

	text_shaders_ = new ShaderHandler(
		"glsl/textshader_vs.glsl",
		"glsl/textshader_fs.glsl"
	);
	gui_shaders_ = new ShaderHandler(
		"glsl/guishader_vs.glsl",
		"glsl/guishader_fs.glsl"
	);
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

	map_handler_.InitializeMaps(
		"../Resources/Map/TestMapMediumHard2.bmp",
		"../Resources/Map/rock.png",
		"../Resources/Map/v4.png");


	hud.LoadHealthBarTexture((char*)"../Resources/GUI/healthbar.png");
	hud.LoadQuickSlotTexture((char*)"../Resources/GUI/quickslot.png");
}

Render::~Render() {
	delete geometry_pass_;
	delete lighting_pass_;
	delete[] lights_;

	delete text_shaders_;
	delete gui_shaders_;

	for (int i = 0; i < nr_of_models_; i++) {
		delete model_[i];
	}
	delete[] model_;
}

void Render::InitializeRender() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	hud.Initiliaze();

	geometry_pass_->GeometryFrameBuffers();

	lights_[0].LightDefault(
		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	map_handler_.InitializeBuffers(geometry_pass_->GetProgram());
}

void Render::UpdateRender(
	float dt, 
	glm::vec3 camera_position,
	glm::mat4 perspective_view_matrix,
	std::vector<ObjectPackage>& object_vector,
	PlayerInfoPackage player_data) {

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	//SET UP FOR 3D
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Pushing Map into object vector
	glm::mat4 map_matrix = glm::mat4(1.0f);
	ObjectPackage map_package;
	map_package.id = OBJECT_ID_MAP;
	map_package.model_matrix = map_matrix;
	object_vector.insert(object_vector.begin(), map_package);

	//  GEOMETRY
	GeometryPass(camera_position, perspective_view_matrix);
	GeometryDrawing(object_vector);
	
	//  LIGHTING
	lights_[0].SetPos(camera_position);
	LightingPass(camera_position);

	RenderQuad();

	//SET UP FOR 2D
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();

	hud.RenderGUI(gui_shaders_, player_data);

	glFlush();
	//swap_buffers(?)
}

void Render::GeometryDrawing(std::vector<ObjectPackage>& object_vector) {
	// object_vector contains all objects which should be drawn
	// we pop it til it runs out of objects.
	glm::vec3 players_position;
	while (!object_vector.empty()) {
		if (OBJECT_ID_NULL == object_vector.back().id) {

		}
		else if (OBJECT_ID_PLAYER == object_vector.back().id) {
			ModelTransformation(object_vector.back().model_matrix);
			players_position = glm::vec3(
				object_vector.back().model_matrix[3][0],
				object_vector.back().model_matrix[3][1],
				object_vector.back().model_matrix[3][2]);
			model_[0]->Draw(geometry_pass_->GetProgram());
		}
		else if (OBJECT_ID_JOHNNY_BRAVO == object_vector.back().id) {
			ModelTransformation(object_vector.back().model_matrix);
		}
		else if (OBJECT_ID_MAP == object_vector.back().id) {
			std::vector<glm::vec2> cells = map_handler_.GridCulling(
				map_handler_.CurrentCell(players_position));
			while (!cells.empty()) {
				ModelTransformation(map_handler_.Transformation((int)cells.back().x, (int)cells.back().y));
				map_handler_.Draw(geometry_pass_->GetProgram(), (int)cells.back().x, (int)cells.back().y);
				cells.pop_back();
			}
		}
		object_vector.pop_back();
	}
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

void Render::RenderMenuState(Menu menu) {
	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	menu.RenderMenu(text_shaders_);
}

void Render::RenderPauseMenu(Menu menu) {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	menu.RenderPauseMenu(text_shaders_);
}

void Render::RenderDeathMenu(Menu menu) {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	menu.RenderDeathMenu(text_shaders_);
}

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
void Render::RenderQuad() {
	if (quad_vertex_array_object_ == 0)
	{
		GLfloat quad_vertices[] = {
			// Positions        // Texture Coords
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quad_vertex_array_object_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

std::vector<std::vector<float>>* Render::GetMapPointer()
{
	return map_handler_.GetMapDataPointer();
}
