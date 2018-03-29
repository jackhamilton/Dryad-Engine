#include "GameLoop.h"
#include <Input.h>


GameLoop::GameLoop(int fps)
{
	running = false;
	frame = 0;
	GameLoop::fps = fps;
}

void GameLoop::start()
{
	running = true;
		while (running) {
			input->handleInput();
			for (list<World>::iterator i = GameLoop::worlds.begin(); i != GameLoop::worlds.end(); i++)
				(*i).render();
			frame++;
			SDL_Delay(50);
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
