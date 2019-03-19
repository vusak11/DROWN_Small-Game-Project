#include "render.h"

Render::Render() {
	quad_vertex_array_object_ = 0;
	quad_vertex_buffer_object_ = 0;

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

	geometry_pass_->GeometryFrameBuffers();
	
}

Render::~Render() {
	delete geometry_pass_;
	delete lighting_pass_;
	delete[] lights_;

	delete text_shaders_;
	delete gui_shaders_;

	//Save the adress to the null model that is deleted first.
	//If that pointer is encountered later in the following loop
	//just set it to null. This prevents crashes by deleting a 
	//non-existant object.
	Model* ptr_to_null = model_[0];

	for (int i = 0; i < nr_of_models_; i++) {
		if (model_[i] == ptr_to_null) { model_[i] = NULL; }
		delete model_[i];
	}
	delete[] model_;
}

void Render::InitializeRender(MetaData* meta_data) {
	//--------------------------------------------------------
	//-------------------Load Map Data------------------------
	//--------------------------------------------------------
	map_handler_.InitializeMaps(
		"../Resources/Map/MainMap_Blocks2.bmp",
		"../Resources/Map/cavewall.png",
		"../Resources/Map/v4.png");
	//--------------------------------------------------------
	//---------------Load Models to Array---------------------
	//--------------------------------------------------------
	//Make space for 1 model per ObjectID
	this->nr_of_models_ = NUMBER_OF_OBJECT_IDS;
	model_ = new Model*[this->nr_of_models_];

	//Link models to a ObjectID
	model_[OBJECT_ID_NULL] = new Model((char*)"../Resources/Models/DefaultNull/defaultNull.obj");

	//!!!
	//Set everything in the game to be the default dummy. This let's us identify missing models
	for (unsigned int i = 0; i < NUMBER_OF_OBJECT_IDS; i++) {
		model_[i] = model_[OBJECT_ID_NULL];
	}
	//!!!

	//Continue setting models
	model_[OBJECT_ID_PLAYER_IDLE] = new Model((char*)"../Resources/Models/Character/Idle/IdleStance.obj");
	model_[OBJECT_ID_PLAYER_JUMP] = new Model((char*)"../Resources/Models/Character/Jump/JumpStance.obj");
	model_[OBJECT_ID_PLAYER_LEFT_WALK_1] = new Model((char*)"../Resources/Models/Character/L1/L1.obj");
	model_[OBJECT_ID_PLAYER_LEFT_WALK_2] = new Model((char*)"../Resources/Models/Character/L2/L2.obj");
	model_[OBJECT_ID_PLAYER_LEFT_WALK_3] = new Model((char*)"../Resources/Models/Character/L3/L3.obj");
	model_[OBJECT_ID_PLAYER_RIGHT_WALK_1] = new Model((char*)"../Resources/Models/Character/R1/R1.obj");
	model_[OBJECT_ID_PLAYER_RIGHT_WALK_2] = new Model((char*)"../Resources/Models/Character/R2/R2.obj");
	model_[OBJECT_ID_PLAYER_RIGHT_WALK_3] = new Model((char*)"../Resources/Models/Character/R3/R3.obj");
	model_[OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_1] = new Model((char*)"../Resources/Models/Character/Attack/Sword/AttackStance1/attackStance1.obj");
	model_[OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_2] = new Model((char*)"../Resources/Models/Character/Attack/Sword/AttackStance2/attackStance2.obj");
	model_[OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_3] = new Model((char*)"../Resources/Models/Character/Attack/Sword/AttackStance3/attackStance3.obj");
	model_[OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_1] = new Model((char*)"../Resources/Models/Character/Attack/Axe/AxeStance1/axeStance1.obj");
	model_[OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_2] = new Model((char*)"../Resources/Models/Character/Attack/Axe/AxeStance2/axeStance2.obj");
	model_[OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_3] = new Model((char*)"../Resources/Models/Character/Attack/Axe/AxeStance3/axeStance3.obj");

	model_[OBJECT_ID_DUMMY] = new Model((char*)"../Resources/Models/NPC/AI.obj");
	model_[OBJECT_ID_FIRE_AI] = new Model((char*)"../Resources/Models/NPC/fireAI.obj");
	model_[OBJECT_ID_WOOD_AI] = new Model((char*)"../Resources/Models/NPC/grassAI.obj");
	model_[OBJECT_ID_ICE_AI] = new Model((char*)"../Resources/Models/NPC/iceAI.obj");
	model_[OBJECT_ID_GHOST] = new Model((char*)"../Resources/Models/NPC/Ghost/ghost.obj");

	model_[OBJECT_ID_DROP_HP_RESTORE] = new Model((char*)"../Resources/Models/Drops/heart_drop/heart_drop.obj");
	model_[OBJECT_ID_DROP_HP_UP] = new Model((char*)"../Resources/Models/Drops/hp_buff/hp_buff.obj");
	model_[OBJECT_ID_DROP_ATK_UP] = new Model((char*)"../Resources/Models/Drops/attack_buff/attack_buff.obj");
	model_[OBJECT_ID_DROP_SPD_UP] = new Model((char*)"../Resources/Models/Drops/speed_buff/speed_buff.obj");
	model_[OBJECT_ID_DROP_DASH] = new Model((char*)"../Resources/Models/Drops/dash/dash.obj");
	model_[OBJECT_ID_DROP_DOUBLE_JUMP] = new Model((char*)"../Resources/Models/Drops/double_jump/double_jump.obj");
	model_[OBJECT_ID_DROP_SWORD] = new Model((char*)"../Resources/Models/Drops/sword/sword.obj");
	model_[OBJECT_ID_DROP_AXE] = new Model((char*)"../Resources/Models/Drops/axe/axe.obj");
	model_[OBJECT_ID_DROP_KEY] = new Model((char*)"../Resources/Models/Drops/key/key.obj");
	model_[OBJECT_ID_DROP_DOOR] = new Model((char*)"../Resources/Models/Drops/Gate/Gate.obj");

	model_[OBJECT_ID_DROP_CHEST_CLOSED] = new Model((char*)"../Resources/Models/Drops/Chest/Closed/chestClosed.obj");
	model_[OBJECT_ID_DROP_CHEST_OPEN] = new Model((char*)"../Resources/Models/Drops/Chest/Open/openChest.obj");

	model_[OBJECT_ID_BOSS] = new Model((char*)"../Resources/Models/Boss/boss.obj");
	model_[OBJECT_ID_BOSS_HAND] = new Model((char*)"../Resources/Models/Boss/bossHand.obj");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	hud_.Initiliaze();
	meta_data_ptr_ = meta_data;

	// Fetch light information and store locally
	nr_of_lights_ = meta_data_ptr_->GetLightPositions().size();
	lights_ = new Light[nr_of_lights_];
	light_positions_ = meta_data_ptr_->GetLightPositions();
	// Set player light
	lights_[0].SetBrightness(glm::vec3(0.1f, 0.1f, 0.1f));
	// Set danger light
	lights_[1].SetPos(glm::vec3(light_positions_[1], 0.0));
	lights_[1].SetAmbientLight(glm::vec3(0.7f, 0.0f, 0.0f));
	lights_[1].SetBrightness(glm::vec3(1.0f, 1.0f, 1.0f));
	// Set boss attack light
	lights_[2].SetPos(glm::vec3(light_positions_[2], 0.0));
	lights_[2].SetBrightness(glm::vec3(1.0f, 1.0f, 1.0f));
	lights_[2].SetAmbientLight(glm::vec3(0.7f, 0.0f, 0.0f));

	// Set colour of the light depending on where in the world it's located, starting at 2 to not affect player light or the "danger light"
	for (int i = 3; i < nr_of_lights_; i++) {
		lights_[i].SetPos(glm::vec3(light_positions_[i], 10.0f));
		if (meta_data_ptr_->GetZone(light_positions_[i]) == RED) {
			lights_[i].SetAmbientLight(glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (meta_data_ptr_->GetZone(light_positions_[i]) == GRE) {
			lights_[i].SetAmbientLight(glm::vec3(0.01f, 0.84f, 0.01f));
		}
		else if (meta_data_ptr_->GetZone(light_positions_[i]) == BLU) {
			lights_[i].SetAmbientLight(glm::vec3(0.0f, 0.4f, 1.0f));
		}
	}
	map_handler_.InitializeBuffers(geometry_pass_->GetProgram());
}

void Render::UpdateMetadata(MetaData* meta_data) {
	this->meta_data_ptr_ = meta_data;
	
	// Set colour of the light depending on where in the world it's located, starting at 2 to not affect player light or the "danger light"
	for (int i = 3; i < nr_of_lights_; i++) {
		lights_[i].SetPos(glm::vec3(light_positions_[i], 10.0f));
		if (meta_data_ptr_->GetZone(light_positions_[i]) == RED) {
			lights_[i].SetAmbientLight(glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (meta_data_ptr_->GetZone(light_positions_[i]) == GRE) {
			lights_[i].SetAmbientLight(glm::vec3(0.01f, 0.84f, 0.01f));
		}
		else if (meta_data_ptr_->GetZone(light_positions_[i]) == BLU) {
			lights_[i].SetAmbientLight(glm::vec3(0.0f, 0.4f, 1.0f));
		}
	}
}

void Render::UpdateRender(
	float dt,
	glm::vec3 camera_position,
	glm::mat4 perspective_view_matrix,
	std::vector<ObjectPackage>& object_vector,
	PlayerInfoPackage player_data,
	bool boss_warning_light_state,
	GameState game_state,
	bool mini_map_enabled) {

	//SET UP FOR 3D
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//  GEOMETRY
	GeometryPass(camera_position, perspective_view_matrix);
	GeometryDrawing(object_vector);

	glm::vec3 player_pos = player_data.position;
	
	//  LIGHTING
	lights_[0].SetPos(glm::vec3(player_pos.x, (player_pos.y + 15.0), 0.0));		//Place players light on our character
	// Update color of players light depending on zone
	if (meta_data_ptr_->GetZone(player_pos) == DEF || game_state == GameState::BOSS) {
		lights_[0].SetAmbientLight(glm::vec3(1.0f, 0.58f, 0.20f));
	}
	else if (meta_data_ptr_->GetZone(player_pos) == RED) {
		lights_[0].SetAmbientLight(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (meta_data_ptr_->GetZone(player_pos) == GRE) {
		lights_[0].SetAmbientLight(glm::vec3(0.01f, 0.84f, 0.01f));
	}
	else if (meta_data_ptr_->GetZone(player_pos) == BLU) {
		lights_[0].SetAmbientLight(glm::vec3(0.0f, 0.4f, 1.0f));
	}
	
	if (player_pos.y < -1700) {
		lights_[0].SetAmbientLight(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	//std::cout << (int)boss_warning_light_state << "\n";
	// Update boss light indication
	if (boss_warning_light_state) {
		lights_[2].SetAmbientLight(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else {
		lights_[2].SetAmbientLight(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	LightingPass(camera_position);

	RenderQuad();

	//SET UP FOR 2D
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();

	hud_.RenderGUI(gui_shaders_, player_data, mini_map_enabled);

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
}

void Render::ModelTransformation(glm::mat4 model_matrix) {
	glUniformMatrix4fv(glGetUniformLocation(geometry_pass_->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
}

void Render::GeometryPass(
	glm::vec3 camera_position,
	glm::mat4 perspective_view_matrix) {
	glViewport(0, 0, 
		GlobalSettings::Access()->ValueOf("WINDOW_WIDTH"), 
		GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT"));
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	for (int i = 0; i < nr_of_lights_; i++) {
		glUniform3f(
			glGetUniformLocation(lighting_pass_->GetProgram(), ("lights[" + std::to_string(i) + "].position").c_str()),
			lights_[i].GetPos().x, lights_[i].GetPos().y, lights_[i].GetPos().z
		);
		glUniform3f(
			glGetUniformLocation(lighting_pass_->GetProgram(), ("lights[" + std::to_string(i) + "].color").c_str()),
			lights_[i].GetAmbientLight().r, lights_[i].GetAmbientLight().g, lights_[i].GetAmbientLight().b
		);
		// Attenuation parameters, and calculate radius
		const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)

		// Save these values to know where we're coming from
		//const float linear = 0.007f;
		//const float quadratic = 0.0002f;

		const float linear = 0.014f;
		const float quadratic = 0.0004f;
		
		glUniform1f(glGetUniformLocation(lighting_pass_->GetProgram(), ("lights[" + std::to_string(i) + "].a_linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(lighting_pass_->GetProgram(), ("lights[" + std::to_string(i) + "].a_quadratic").c_str()), quadratic);
		// then calculate radius of light volume/sphere
		const float max_brightness = std::fmaxf(std::fmaxf(lights_[i].GetBrightness().r, lights_[i].GetBrightness().g), lights_[i].GetBrightness().b);
		float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * max_brightness))) / (2.0f * quadratic);
		glUniform1f(glGetUniformLocation(lighting_pass_->GetProgram(), ("lights[" + std::to_string(i) + "].radius").c_str()), radius);
	}
	glUniform3fv(glGetUniformLocation(lighting_pass_->GetProgram(), "view_pos"), 1, glm::value_ptr(camera_position)); //Add camera positions
}

void Render::RenderMenuState(Menu menu) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	menu.RenderMenu(text_shaders_);
}

void Render::RenderOptionsMenu(Menu menu, CameraHandler* cam_handler) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	menu.RenderOptionsMenu(text_shaders_, cam_handler);
}

void Render::RenderPauseMenu(Menu menu) {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	menu.RenderPauseMenu(text_shaders_);
}

void Render::RenderDeathMenu(Menu menu) {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	menu.RenderDeathMenu(text_shaders_);
}

void Render::RenderVictoryMenu(Menu menu) {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	menu.RenderVictoryMenu(text_shaders_);
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
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
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