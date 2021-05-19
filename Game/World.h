#pragma once
#include <map>
#include <vector>
#include <memory>
#include <string>
#include "Scene.h"
#include "Mouse.h"
#include "Input.h"
#include "Text.h"
#include "Camera.h"
#include "AssetLibrary.h"

using namespace std;
class World
{
public:
	World(shared_ptr<Renderer> renderer, shared_ptr<Input> input);
	void addScene(shared_ptr<Scene> s, string name);
	void setScene(string name);
	string getCurrentSceneName();
	shared_ptr<Scene> getCurrentScene();
	void setRenderer(shared_ptr<Renderer> renderer);
	void render(int frame, int fps);
	void renderGameObject(shared_ptr<GameObject> obj, Point positionMod, double MSPerFrame);
	shared_ptr<Renderer> getRenderer();

	//debug layer
	void setDisplayFPS(bool enabled, shared_ptr<AssetLibrary> assetLibrary);
	void setDisplayObjectCount(bool enabled, shared_ptr<AssetLibrary> assetLibrary);
	void setDisplayHitboxes(bool enabled);
	string getDebugFont();
	int getDebugFontSize();
	//Adds an object to the superimposed debug layer
	void addDebugObject(shared_ptr<GameObject> g);
private:
	shared_ptr<Camera> camera;
	shared_ptr<Input> input;
	shared_ptr<Mouse> mouse;
	shared_ptr<Renderer> renderer;
	map<string, shared_ptr<Scene>> scenes;
	shared_ptr<Scene> currentScene;
	clock_t lastLocalTime = 0;

	//debug layer
	shared_ptr<long double> lastFrameTimeMS;
	string debugFont = "OpenSans-Regular";
	int debugFontSize = 14;
	shared_ptr<long double> defaultFps;
	int cFPS;
	int potentialFPS;
	bool displayFPS;
	bool displayObjectCount;
	bool displayHitboxes;

	void refreshFPS();
	void refreshObjectCount();

	shared_ptr<Text> fpsIndicator;
	shared_ptr<Text> objectCountIndicator;
	shared_ptr<AssetLibrary> assetLibrary;
	vector<shared_ptr<GameObject>> debugObjects;

	friend class GameLoop;
};

