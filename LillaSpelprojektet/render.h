#pragma once
class Render {
private:
	void DrawScene();
public:
	Render();
	~Render();

	void InitilizeRender();
	void UpdateRender(float dt);
};

