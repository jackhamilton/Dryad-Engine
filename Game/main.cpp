#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Spritesheet.h"
#include "Scene.h"
#include "GameLoop.h"
#include "Input.h"
#include <GameObject.h>
#include <Point.h>

using namespace std;

static GameLoop gameLoop;
static Spritesheet* linkSpr;
static GameObject* link;

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
	strcat(s, "res/zelda.png");
    std::vector<char*> images = { s };
    std::vector<int> heights = { 130, 130, 130, 130, 130, 130, 130, 130 };
    std::vector<int> framecounts = { 3, 3, 1, 3, 10, 10, 10, 10 };
	linkSpr = new Spritesheet(images, framecounts, heights, 120);
	linkSpr->setCurrentAnimation(0);
	link = new GameObject(Point(50, 50), linkSpr);
	world.addObject(link);
	//Configure inputs
	input.addKeyboardEvent([]() { gameLoop.stop(); }, { make_pair(SDLK_ESCAPE, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() { 
		Point p = link->getLocation();
		p.x += 7.0*fpsSpeedFactor;
		link->setLocation(p);
		((Spritesheet*)link->getSprite())->setCurrentAnimation(7);
	}, { make_pair(SDLK_RIGHT, SDL_KEYDOWN)});
	input.addKeyboardEvent([]() {
		Point p = link->getLocation();
		p.x -= 7.0*fpsSpeedFactor;
		link->setLocation(p);
		((Spritesheet*)link->getSprite())->setCurrentAnimation(5);
	}, { make_pair(SDLK_LEFT, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() {
		((Spritesheet*)link->getSprite())->setCurrentAnimation(0);
	}, { make_pair(SDLK_LEFT, SDL_KEYUP),
		 make_pair(SDLK_RIGHT, SDL_KEYUP) }); //If all are up, reset animation
	gameLoop.setInput(&input);

	gameLoop.addWorld(world);
	gameLoop.start();
	window->destroy();
	world.destroy();
	IMG_Quit();
	return 0;
	//Missing PNG libraries: libpng, libz
}
