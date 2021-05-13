#pragma once
#include <list>
#include "Scene.h"
#include "World.h"

class Input;

class GameLoop {
public:
	GameLoop(World* world, Input* input) : GameLoop(60, world, input) {};
	GameLoop(int fps, World* world, Input* input);
	void start();
	void stop();
	void setInput(Input* input);
	void setWorld(World* world);
	bool running;
	void destroy();
private:
	Input* input;
	int frame;
	int fps;
	vector<long double> cFPS;
	vector<long double> cPotentialFPS;
	long double frameTimeMS;
	long double lastFrameTimeMS;
	World* world;
	void logFPS();
};
