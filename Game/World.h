#pragma once
#include <map>
#include "Scene.h"
#include "Mouse.h"
#include "Input.h"

using namespace std;
class World
{
public:
	World(Renderer* renderer, Input* input);
	void addScene(Scene* s, const char* name);
	void setScene(const char* name);
	string getCurrentSceneName();
	Scene* getCurrentScene();
	void setRenderer(Renderer* renderer);
	void render(int frame, int fps);
	Renderer* getRenderer();
	void destroy();
	void setDisplayFPS(bool enabled);
private:
	Input* input;
	Mouse* mouse;
	Renderer* renderer;
	map<const char*, Scene*> scenes;
	Scene* currentScene;
	bool displayFPS;
	GameObject* fpsIndicator;
};

