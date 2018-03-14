#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"

using namespace std;

int main(int argc, char* args[]) {
	Window window = Window("Hello World", 1280, 720);
	system("PAUSE");
	window.destroy();
	return 0;
}