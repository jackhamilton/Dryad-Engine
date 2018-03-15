#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"

using namespace std;

int main(int argc, char* args[]) {
	Window window = Window("Hello World", ml16_9);
	SDL_Delay(2000);
	window.changeWindowResolution(l16_9);
	window.setFullscreen(true);
	system("PAUSE");
	window.destroy();
	return 0;
}