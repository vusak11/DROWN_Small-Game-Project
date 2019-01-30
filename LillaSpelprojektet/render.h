#pragma once
class render {
private:
	void DrawScene();
public:
	render();
	~render();

	void InitilizeRender();
	void UpdateRender(float dt);
};

