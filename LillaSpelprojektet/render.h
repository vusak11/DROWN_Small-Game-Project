#pragma once
#include "shader_handler.h"

class Render {
private:
	void DrawScene();
	ShaderHandler* geometry_pass_;
public:
	Render();
	~Render();

	void InitilizeRender();
	void UpdateRender(float dt);
};

