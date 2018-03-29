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
		input->handleInput();
		for (list<World>::iterator i = GameLoop::worlds.begin(); i != GameLoop::worlds.end(); i++)
			(*i).render(frame, fps);
		frame++;
		long double endTimeMS = time(0) * 1000;
		long double renderTimeTakenMS = endTimeMS - sysTimeMS;
		SDL_Delay((Uint32)(frameTimeMS - renderTimeTakenMS));
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

void GameLoop::addWorld(World world)
{
	worlds.push_back(world);
}

void GameLoop::destroy()
{
	delete input;
}
