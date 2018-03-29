#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include "Window.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Spritesheet.h"
#include "Scene.h"
#include "GameLoop.h"
#include "Input.h"

using namespace std;

static GameLoop gameLoop;
static Spritesheet* link;

static int fps = 120;
static double fpsSpeedFactor;

int main(int argc, char* args[]) {

    Window* window = new Window("Dryad", m16_9);
    Scene world;
    world.setRenderer(window->getRenderer());
	fpsSpeedFactor = 60.0 / (double)fps;;
	Input input;
	gameLoop = GameLoop(fps);
	char* s = SDL_GetBasePath();
	//strcat(s, "\\");
	strcat(s, "res/zelda.png");
    std::vector<char*> images = { s };
    std::vector<int> heights = { 130, 130, 130, 130, 130, 130, 130, 130 };
    std::vector<int> framecounts = { 3, 3, 1, 3, 10, 10, 10, 10 };
	link = new Spritesheet(images, framecounts, heights, 120, &world);
	link->setCurrentAnimation(0);

	//Configure inputs
	input.addKeyboardEvent([]() { gameLoop.stop(); }, SDL_KEYDOWN, { SDLK_ESCAPE });
	input.addKeyboardEvent([]() { 
		Point p = link->getLocation();
		p.x += 7.0*fpsSpeedFactor;
		link->setLocation(p);
		link->setCurrentAnimation(7);
	}, SDL_KEYDOWN, { SDLK_RIGHT });
	input.addKeyboardEvent([]() {
		Point p = link->getLocation();
		p.x -= 7.0*fpsSpeedFactor;
		link->setLocation(p); 
		link->setCurrentAnimation(5);
	}, SDL_KEYDOWN, { SDLK_LEFT });
	input.addKeyboardEvent([]() {
		Point p = link->getLocation();
		p.y += 7*fpsSpeedFactor;
		link->setLocation(p);
		link->setCurrentAnimation(4);
	}, SDL_KEYDOWN, { SDLK_DOWN });
	input.addKeyboardEvent([]() {
		Point p = link->getLocation();
		p.y -= 7.0*fpsSpeedFactor;
		link->setLocation(p);
		link->setCurrentAnimation(6);
	}, SDL_KEYDOWN, { SDLK_UP });
	input.addKeyboardEvent([](){
		link->setCurrentAnimation(0);
		}, SDL_KEYUP, { SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN });
	gameLoop.setInput(&input);

	gameLoop.addWorld(world);
	gameLoop.start();
	window->destroy();
	world.destroy();
	IMG_Quit();
	return 0;
	//Missing PNG libraries: libpng, libz
}
