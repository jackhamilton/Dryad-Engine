#pragma once
#include <World.h>
#include <list>

class Input;

class GameLoop {
public:
	GameLoop(int fps);
	void start();
	void stop();
	void setInput(Input* input);
	void addWorld(World world);
	list<World> worlds;
	bool running;
	void destroy();
private:
	Input* input;
	int frame;
	int fps;
};