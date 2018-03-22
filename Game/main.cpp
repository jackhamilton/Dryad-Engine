#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include <Renderer.h>
#include <Sprite.h>
#include <Spritesheet.h>

using namespace std;

int main(int argc, char* args[]) {
	Window* window = new Window("Hello World", m16_9);
	window->setRenderer(new Renderer(window));
	char* s = SDL_GetBasePath();
	strcat(s, "\\");
	strcat(s, "res\\zelda.BMP");
	const char* images[] = { s };
	int heights[] = { 130, 130, 130, 130, 130, 130, 130, 130 };
	Spritesheet* chase = new Spritesheet(images, heights, 120, window);
	int* dim = new int[2];
	chase->setCurrentAnimation(7);
	chase->getCurrentFrame(dim);
	cout << dim[0] <<", " << dim[1];
	window->render();
	SDL_Delay(200);
	window->render();
	SDL_Delay(200);
	window->render();
	SDL_Delay(200);
	window->render();
	SDL_Delay(200);
	window->render();
	SDL_Delay(200);
	window->render();
	//image array size goes super weird after rendering
	system("PAUSE");
	window->destroy();
	return 0;
	//Missing PNG libraries: libpng, libz
}