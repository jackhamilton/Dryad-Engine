#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include <Renderer.h>
#include <Sprite.h>

using namespace std;

int main(int argc, char* args[]) {
	Window* window = new Window("Hello World", ml16_9);
	window->setRenderer(new Renderer(window));
	const char* images[] = { "Z:\\Pictures\\test.bmp" };
	Sprite* chase = new Sprite(images, window);
	window->render();
	//image array size goes super weird after rendering
	system("PAUSE");
	window->destroy();
	return 0;
	//Missing PNG libraries: libpng, libz
}