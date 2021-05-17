#pragma once
#include <list>
#include "Scene.h"
#include "World.h"
#include <memory>
#include <functional>

class Input;

class GameLoop {
public:
	GameLoop(shared_ptr<World> world, shared_ptr<Input> input) : GameLoop(60, world, input) {};
	GameLoop(int fps, shared_ptr<World> world, shared_ptr<Input> input);
	void start();
	void stop();
	void setInput(shared_ptr<Input> input);
	void setWorld(shared_ptr<World> world);
	bool running;
private:
	shared_ptr<Input> input;
	int frame;
	int fps;
	vector<long double> cFPS;
	vector<long double> cPotentialFPS;
	long double frameTimeMS;
	long double lastFrameTimeMS;
	shared_ptr<World> world;
	void logFPS();
};
