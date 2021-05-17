#include "GameLoop.h"
#include "Input.h"
#include <time.h>

GameLoop::GameLoop(int fps, shared_ptr<World> world, shared_ptr<Input> input)
{
	GameLoop::world = world;
	running = false;
	frame = 0;
	GameLoop::fps = fps;
	//calculate the time per frame
	GameLoop::frameTimeMS = (1 / ((double)fps)) * 1000;
	GameLoop::input = input;
	world->lastFrameTimeMS = make_shared<long double>(lastFrameTimeMS);
	world->defaultFps = make_shared<long double>(GameLoop::fps);
}

void GameLoop::start()
{
	running = true;
	while (running) {
		clock_t sysTime = clock();
		input->handleInput(this);
		world->render(frame, fps);
		if (frame % 10 == 0) {
			//every 10 frames
			if (world->displayFPS) {
				logFPS();
			}
		}
		frame++;
		clock_t endTime = clock();
		long double renderTimeTakenMS = ((long double)(endTime - sysTime))/(CLOCKS_PER_SEC/1000);
		GameLoop::cPotentialFPS.push_back((long double)(endTime - sysTime));
		//TODO: scene preloading will go here. ALSO PUT SPRITE LOADER HERE (for stuff like fps that must be reloaded. anything with !loaded)
		if (frameTimeMS - renderTimeTakenMS > 0) {
			SDL_Delay((Uint32)(frameTimeMS - renderTimeTakenMS));
		}
		clock_t actualEndTime = clock();
		cFPS.push_back((long double)(actualEndTime - sysTime));
		lastFrameTimeMS = (actualEndTime - sysTime) / (CLOCKS_PER_SEC / 1000);
		frame++;
	}
}

void GameLoop::logFPS() {
	//log current fps
	long double total = 0;
	long double count = 0;
	for (long double i : cFPS) {
		total += i;
		count++;
	}
	total /= (CLOCKS_PER_SEC/1000);
	total /= count;
	world->cFPS = (int)(1000/total);
	cFPS.clear();
	//log potential fps
	total = 0;
	count = 0;
	for (long double i : cPotentialFPS) {
		total += i;
		count++;
	}
	total /= (CLOCKS_PER_SEC / 1000);
	total /= count;
	world->potentialFPS = (int)(1000/total);
	cPotentialFPS.clear();
	world->refreshFPS();
}

void GameLoop::stop()
{
	running = false;
}

void GameLoop::setInput(shared_ptr<Input> input) {
	GameLoop::input = input;
}

void GameLoop::setWorld(shared_ptr<World> world)
{
	GameLoop::world = world;
}