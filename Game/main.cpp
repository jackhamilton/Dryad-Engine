#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include <Renderer.h>
#include <Sprite.h>
#include <Spritesheet.h>
#include <World.h>
#include <GameLoop.h>

using namespace std;

int main(int argc, char* args[]) {
	Window* window = new Window("Hello World", m16_9);
	World world;
	world.setRenderer(new Renderer(window));
	char* s = SDL_GetBasePath();
	strcat(s, "\\");
	strcat(s, "res\\zelda.BMP");
	const char* images[] = { s };
	int heights[] = { 130, 130, 130, 130, 130, 130, 130, 130 };
	Spritesheet* chase = new Spritesheet(images, heights, 120, &world);
	chase->setCurrentAnimation(7);
	//image array size goes super weird after rendering
	GameLoop gameLoop;
	gameLoop.addWorld(world);
	gameLoop.start();
	window->destroy();
	world.destroy();
	return 0;
	//Missing PNG libraries: libpng, libz
}