#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include <Renderer.h>
#include <Sprite.h>

using namespace std;

int main(int argc, char* args[]) {
	Window window = Window("Hello World", ml16_9);
	window.setRenderer(new Renderer(window));
	char images[] = { "Z:\\Pictures\\chaseistrash.png" };
	Sprite* chase = new Sprite(images, window);
	window.render();
	SDL_Delay(2000);
	window.changeWindowResolution(s16_9);
	system("PAUSE");
	window.destroy();
	return 0;
}