#include <iostream>

#include <thread>

#include "Core/Game.h"
#include "GLDebug.h"

#include <SFML/OpenGL.hpp>
#include "global_settings.h"

#define _CRTDBG_MAP_ALLOC 

void LoadingLoop(
	Game& in_game,
	sf::Window& in_window);

void GameLoop(
	const bool& in_running,
	Game& in_game,
	sf::Window& in_window);

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GlobalSettings::Access()->UpdateValuesFromFile();

	sf::Window window(sf::VideoMode(
		GlobalSettings::Access()->ValueOf("WINDOW_WIDTH"),
		GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT")),
		"Drown", 
		sf::Style::Default, 
		sf::ContextSettings(32));
	float test = GlobalSettings::Access()->ValueOf("WINDOW_WIDTH");
	window.setVerticalSyncEnabled(true);

	window.setActive(true);
	 
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not linking" << std::endl;
	}

	/*----------Start GL Debugging----------*/
	//EnableGLDebug(); //Comment this away to stop output
	/*----------End of Start GL Debugging----------*/

	/*----------Variables----------*/
	Game game;
	bool running = true;
	/*----------End of Variables----------*/
	
	/*-----------Initialize---------------*/
	
	//Start loading screen
	/*std::thread loading_thread(
		&LoadingLoop,
		std::ref(game),
		std::ref(window)
	);*/

	LoadingLoop(std::ref(game), std::ref(window));

	//Start the game
	game.InitializeGame();
	
	//Wait for loading screen to finish
	//loading_thread.join();

	//Start thread iterating game
	std::thread game_thread(
		&GameLoop,
		std::ref(running),
		std::ref(game),
		std::ref(window)
	);

	//Tell this thread to not use the window actively
	window.setActive(false);

	//Turn of repeating key presses
	window.setKeyRepeatEnabled(false);

	/*-----------End Initialize---------------*/
	while (game.IsRunning()) {
		sf::Event event;
		
		while (window.pollEvent(event)) {
			/*----------------Only exit window commands-----------*/
			if (event.type == sf::Event::Closed) {
				running = false;
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
			/*----------------Only exit window commands-----------*/
			/*----------------Input from mouse / keyboard---------*/
			game.InputEvents(event);
			/*----------------Input from mouse / keyboard---------*/
		}

		game.InputContinual();
	}

	game_thread.join();	//wait for thread to finish

	return 0;
}

void LoadingLoop(
	Game& in_game,
	sf::Window& in_window) {

	//in_window.setActive(false);
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexCoord;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"	TexCoord = aTexCoord;\n"
		"}\0";

	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"void main()\n"
		"{\n"
		"   FragColor = texture(texture1, TexCoord);\n"
		"}\n\0";


	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex_shader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	//Def what you need
	GLfloat vertices[] = {
		// Positions        // Texture Coords
		-1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		0, 2, 3   // second Triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int tex_width, tex_height;
	unsigned char* image = SOIL_load_image("../Resources/LoadingScreen/loadingscreen.png", &tex_width, &tex_height, 0, SOIL_LOAD_RGB);
	
	std::cout << "Loading Screen: " << "../Resources/LoadingScreen/loadingscreen.png" << SOIL_last_result() << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image);

	glUseProgram(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);


	//while (!in_game.IsLoaded()) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//use what you need

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(shader_program);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		in_window.setActive(false);
		in_window.display();
		in_window.setActive(true);
	//}
	//remove any trash
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
}

void GameLoop(
	const bool& in_running,
	Game& in_game,
	sf::Window& in_window) {

	while (in_game.IsRunning()) {
		in_game.GameIteration();
		in_window.display();
	}
}