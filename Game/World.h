#pragma once
#include <map>
#include <vector>
#include "Scene.h"
#include "Mouse.h"
#include "Input.h"
#include "Text.h"

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
	void setDisplayObjectCount(bool enabled);
	//Adds an object to the superimposed debug layer
	void addDebugObject(GameObject* g);
	const char* getDebugFont();
	int getDebugFontSize();
private:
	Input* input;
	Mouse* mouse;
	Renderer* renderer;
	map<const char*, Scene*> scenes;
	Scene* currentScene;

	//debug layer
	const char* debugFont = "OpenSans-Regular.ttf";
	int debugFontSize = 14;
	bool displayFPS;
	int cFPS;
	int potentialFPS;
	bool displayObjectCount;

	void refreshFPS();
	void refreshObjectCount();

	Text* fpsIndicator;
	Text* objectCountIndicator;
	vector<GameObject*> debugObjects;

	friend class GameLoop;
};

