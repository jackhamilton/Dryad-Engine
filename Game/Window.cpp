#include <stdio.h>
#include <SDL.h>
#include <Window.h>

using namespace std;

int screenWidth;
int screenHeight;

SDL_Window* window;

Window::Window(const char *title, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	window = NULL; 
	//The surface contained by the window 
	SDL_Surface* screenSurface = NULL;
	if(SDL_Init( SDL_INIT_VIDEO ) < 0) { 
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError()); 
	}
	else {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			screenSurface = SDL_GetWindowSurface(window);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);
		}
	}
}

void Window::destroy() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}