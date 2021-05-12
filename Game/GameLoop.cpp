#include "GameLoop.h"
#include "Input.h"
#include <ctime>

GameLoop::GameLoop(int fps, World* world, Input* input)
{
	GameLoop::world = world;
	running = false;
	frame = 0;
	GameLoop::fps = fps;
	//calculate the time per frame
	GameLoop::frameTimeMS = (1 / ((double)fps)) * 1000;
	GameLoop::input = input;
}

void GameLoop::start()
{
	running = true;
	while (running) {
		long double sysTimeMS = (long double)(time(0) * 1000);
		input->handleInput(this);
		world->render(frame, fps);
		frame++;
		long double endTimeMS = (long double)(time(0) * 1000);
		long double renderTimeTakenMS = endTimeMS - sysTimeMS;
		//TODO: scene preloading will go here
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

void GameLoop::setWorld(World* world)
{
	GameLoop::world = world;
}

void GameLoop::destroy()
{
	delete input;
}
