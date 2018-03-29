#include "GameLoop.h"
#include <Input.h>
#include <ctime>

GameLoop::GameLoop(int fps)
{
	running = false;
	frame = 0;
	GameLoop::fps = fps;
	//calculate the time per frame
	GameLoop::frameTimeMS = (1 / ((double)fps)) * 1000;
}

void GameLoop::start()
{
	running = true;
	while (running) {
		long double sysTimeMS = time(0) * 1000;
		input->handleInput(this);
		for (list<Scene>::iterator i = GameLoop::worlds.begin(); i != GameLoop::worlds.end(); i++)
			(*i).render(frame, fps);
		frame++;
		long double endTimeMS = time(0) * 1000;
		long double renderTimeTakenMS = endTimeMS - sysTimeMS;
		if (frameTimeMS - renderTimeTakenMS > 0) {
			SDL_Delay((Uint32)(frameTimeMS - renderTimeTakenMS));
		}
		else {
			SDL_Delay((Uint32)frameTimeMS);
		}
		frame++;
	}
}

void GameLoop::stop()
{
	running = false;
}

void GameLoop::setInput(Input* input) {
	GameLoop::input = input;
}

void GameLoop::addWorld(Scene world)
{
	worlds.push_back(world);
}

void GameLoop::destroy()
{
	delete input;
}
