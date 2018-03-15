#include <stdio.h>
#include <SDL.h>
#include <Window.h>

using namespace std;

int screenWidth;
int screenHeight;
Resolution res;

SDL_Window* window;

void Window::calculateResolution(Resolution res, int &width, int &height) {
	switch (res) {
	case xxs4_3: width = 640, height = 480; break;
	case xs4_3: width = 800, height = 600; break;
	case s4_3: width = 1024, height = 768; break;
	case m4_3: width = 1280, height = 960; break;
	case sl4_3: width = 1440, height = 1080; break;
	case ml4_3: width = 1600, height = 1200; break;
	case l4_3: width = 1920, height = 1440; break;
	case s16_10: width = 1280, height = 800; break;
	case m16_10: width = 1440, height = 900; break;
	case sl16_10: width = 1680, height = 1050; break;
	case l16_10: width = 1920, height = 1440; break;
	case xl16_10: width = 2560, height = 1600; break;
	case s16_9: width = 1024, height = 567; break;
	case m16_9: width = 1280, height = 720; break;
	case sl16_9: width = 1336, height = 768; break;
	case ml16_9: width = 1600, height = 900; break;
	case l16_9: width = 1920, height = 1080; break;
	case xl16_9: width = 2560, height = 1440; break;
	case xxl16_9: width = 3480, height = 2160; break;
	}
}

Window::Window(const char *title, int width, int height, Resolution res) {
	if (res != NULL) {
		calculateResolution(res, width, height);
	}
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
			/*
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);*/
		}
	}
}

void Window::changeWindowResolution(Resolution res) {
	changeWindowResolution(res, 0, 0);
}

void Window::changeWindowResolution(int width, int height) {
	changeWindowResolution((Resolution)NULL, width, height);
}


/*
enum Resolution {
//This is most of the standard screen resolutions for various aspect ratios.
//xxs = extra extra small, ml means medium large (smaller than large), etc.

};
*/

void Window::changeWindowResolution(Resolution res, int width, int height)
{
	if (res != NULL) {
		calculateResolution(res, width, height);
	}
	SDL_SetWindowSize(window, width, height);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Window::setFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(window, fullscreen);
}

void Window::destroy() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}