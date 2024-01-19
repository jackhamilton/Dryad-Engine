#pragma once
#include <list>
#include "Scene.h"
#include "World.h"
#include <memory>
#include <functional>

namespace dryad
{

class Input;

class GameLoop {
public:
	GameLoop(std::shared_ptr<World> world, std::shared_ptr<Input> input) : GameLoop(60, world, input) {};
	GameLoop(int fps, std::shared_ptr<World> world, std::shared_ptr<Input> input);
	void start();
	void stop();
	void setInput(std::shared_ptr<Input> input);
	void setWorld(std::shared_ptr<World> world);
	bool running;
private:
	std::shared_ptr<Input> input;
	int frame;
	int fps;
	std::vector<long double> cFPS;
	std::vector<long double> cPotentialFPS;
	long double frameTimeMS;
	long double lastFrameTimeMS;
	std::shared_ptr<World> world;
	void logFPS();
};

}

