#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include <Renderer.h>
#include <Sprite.h>
#include <Spritesheet.h>
#include <World.h>
#include <GameLoop.h>
#include <Input.h>

using namespace std;

static GameLoop gameLoop;
static Spritesheet* link;

int main(int argc, char* args[]) {
	Window* window = new Window("Hello World", m16_9);
	World world;
	Input input;
	gameLoop = GameLoop(60);
	world.setRenderer(new Renderer(window));
	char* s = SDL_GetBasePath();
	strcat(s, "\\");
	strcat(s, "res\\zelda.BMP");
	const char* images[] = { s };
	int heights[] = { 130, 130, 130, 130, 130, 130, 130, 130 };
	link = new Spritesheet(images, heights, 120, &world);
	link->setCurrentAnimation(4);

	//Configure inputs
	input.addKeyboardEvent([]() { gameLoop.stop(); }, SDL_KEYDOWN, { SDLK_ESCAPE });
	input.addKeyboardEvent([]() { 
		Point p = link->getLocation();
		p.x += 7;
		link->setLocation(p);
		link->setCurrentAnimation(7);
	}, SDL_KEYDOWN, { SDLK_RIGHT });
	input.addKeyboardEvent([]() {
		Point p = link->getLocation();
		p.x -= 7;
		link->setLocation(p); 
		link->setCurrentAnimation(5);
	}, SDL_KEYDOWN, { SDLK_LEFT });
	input.addKeyboardEvent([](){
		link->setCurrentAnimation(4);
		}, SDL_KEYUP, { SDLK_LEFT, SDLK_RIGHT });
	gameLoop.setInput(&input);

	gameLoop.addWorld(world);
	gameLoop.start();
	window->destroy();
	world.destroy();
	return 0;
	//Missing PNG libraries: libpng, libz
}