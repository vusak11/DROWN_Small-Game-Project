#include "render.h"

void Render::DrawScene() {
}

Render::Render() {
	quad_vertex_array_object_ = 0;
	quad_vertex_buffer_object_ = 0;
	nr_of_lights_ = 1;

	//--------------------------------------------------------
	//-------------------Create Shaders-----------------------
	//--------------------------------------------------------
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

	//--------------------------------------------------------
	//-------------------Load Map Data------------------------
	//--------------------------------------------------------
	map_handler_.InitializeMaps(
		"../Resources/Map/MainMapTest_Vel.bmp",
		"../Resources/Map/cavewall.png",
		"../Resources/Map/v4.png");


	//--------------------------------------------------------
	//---------------------Load HUD---------------------------
	//--------------------------------------------------------
	hud_.LoadHealthBarTexture((char*)"../Resources/GUI/healthbar.png");
	hud_.LoadQuickSlotTexture((char*)"../Resources/GUI/quickslot.png");
	

	//--------------------------------------------------------
	//---------------Load Models to Array---------------------
	//--------------------------------------------------------
	//Make space for 1 model per ObjectID
	this->nr_of_models_ = NUMBER_OF_OBJECT_IDS;
	model_ = new Model*[this->nr_of_models_];
	
	//Link models to a ObjectID
	model_[OBJECT_ID_NULL] = new Model((char*)"../Resources/Models/DefaultDummyNPC/defaultDummyNPC.obj");
	model_[OBJECT_ID_PLAYER] = new Model((char*)"../Resources/Models/TestBox/testBOX.obj");
	model_[OBJECT_ID_DUMMY] = new Model((char*)"../Resources/Models/DummyNPC/dummyNPC.obj");
	
	model_[OBJECT_ID_DROP_HP_RESTORE] = new Model((char*)"../Resources/Models/Drops/heart_drop/heart_drop.obj");
	model_[OBJECT_ID_DROP_HP_UP] = new Model((char*)"../Resources/Models/Drops/hp_buff/hp_buff.obj");
	model_[OBJECT_ID_DROP_ATK_UP] = new Model((char*)"../Resources/Models/Drops/attack_buff/attack_buff.obj");
	model_[OBJECT_ID_DROP_DASH] = new Model((char*)"../Resources/Models/Drops/dash/dash.obj");
	model_[OBJECT_ID_DROP_DOUBLE_JUMP] = new Model((char*)"../Resources/Models/Drops/double_jump/double_jump.obj");
	model_[OBJECT_ID_DROP_SWORD] = new Model((char*)"../Resources/Models/Drops/sword/sword.obj");
	model_[OBJECT_ID_DROP_AXE] = new Model((char*)"../Resources/Models/Drops/axe/axe.obj");

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
	
	hud_.Initiliaze();

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
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	hud_.RenderGUI(gui_shaders_, player_data);

	glFlush();
	//swap_buffers(?)
}

void Render::GeometryDrawing(std::vector<ObjectPackage>& object_vector) {
	
	//--------------------DRAW OBJECTS------------------------

	//Loop through all ObjectPackage:s and draw the corresponding models
	ObjectID temp_id;
	for (unsigned int i = 0; i < object_vector.size(); i++) {
		//Upload the object's model-matrix
		this->ModelTransformation(object_vector[i].model_matrix);

		//Get the object's id and draw a model of that type
		temp_id = object_vector[i].id;
		this->model_[temp_id]->Draw(geometry_pass_->GetProgram());
	}
	
	//----------------DRAW THE MAP----------------------------

	//We know the player object to be the first in the vector
	glm::vec3 player_position = glm::vec3(
		object_vector.at(0).model_matrix[3][0],
		object_vector.at(0).model_matrix[3][1],
		object_vector.at(0).model_matrix[3][2]);

	//Then only retrieve cells nearby
	std::vector<glm::vec2> cells = map_handler_.GridCulling(
		map_handler_.CurrentCell(player_position));
	
	//Draw the map
	while (!cells.empty()) {
		ModelTransformation(map_handler_.Transformation((int)cells.back().x, (int)cells.back().y));
		map_handler_.Draw(geometry_pass_->GetProgram(), (int)cells.back().x, (int)cells.back().y);
		cells.pop_back();
	}
	
	/*
	else if (OBJECT_ID_MAP == object_vector.back().id) {
			std::vector<glm::vec2> cells = map_handler_.GridCulling(
				map_handler_.CurrentCell(players_position));
			while (!cells.empty()) {
				ModelTransformation(map_handler_.Transformation((int)cells.back().x, (int)cells.back().y));
				map_handler_.Draw(geometry_pass_->GetProgram(), (int)cells.back().x, (int)cells.back().y);
				cells.pop_back();
			}
		}
		---
		players_position = glm::vec3(
				object_vector.back().model_matrix[3][0],
				object_vector.back().model_matrix[3][1],
				object_vector.back().model_matrix[3][2]);
	*/
}

void Render::ModelTransformation(glm::mat4 model_matrix) {
	glUniformMatrix4fv(glGetUniformLocation(geometry_pass_->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
}

void Render::GeometryPass(
	glm::vec3 camera_position,
	glm::mat4 perspective_view_matrix) {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

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
