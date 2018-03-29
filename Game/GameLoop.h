#pragma once
#include <Scene.h>
#include <list>

class Input;

class GameLoop {
public:
	GameLoop() : GameLoop(60) {};
	GameLoop(int fps);
	void start();
	void stop();
	void setInput(Input* input);
	void addWorld(Scene world);
	list<Scene> worlds;
	bool running;
	void destroy();
private:
	Input* input;
	int frame;
	int fps;
	double frameTimeMS;
};