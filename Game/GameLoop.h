#pragma once
#include <World.h>
#include <list>

class GameLoop {
public:
	GameLoop();
	void start();
	void stop();
	void addWorld(World world);
	list<World> worlds;
	bool running;
};