#include "GameLoop.h"

GameLoop::GameLoop()
{
	running = false;
}

void GameLoop::start()
{
	running = true;
		while (running) {
			for (list<World>::iterator i = GameLoop::worlds.begin(); i != GameLoop::worlds.end(); i++)
				(*i).render();
			SDL_Delay(50);
		}
}

void GameLoop::stop()
{
	running = false;
}

void GameLoop::addWorld(World world)
{
	worlds.push_back(world);
}
