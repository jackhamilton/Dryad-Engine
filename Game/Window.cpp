#include "Window.h"
#include "Renderer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

using namespace std;


pair<int, int> Window::calculateResolution(Resolution res) {
	int width, height;
	switch (res) {
	case Resolution::xxs4_3: width = 640, height = 480; break;
	case Resolution::xs4_3: width = 800, height = 600; break;
	case Resolution::s4_3: width = 1024, height = 768; break;
	case Resolution::m4_3: width = 1280, height = 960; break;
	case Resolution::sl4_3: width = 1440, height = 1080; break;
	case Resolution::ml4_3: width = 1600, height = 1200; break;
	case Resolution::l4_3: width = 1920, height = 1440; break;
	case Resolution::s16_10: width = 1280, height = 800; break;
	case Resolution::m16_10: width = 1440, height = 900; break;
	case Resolution::sl16_10: width = 1680, height = 1050; break;
	case Resolution::l16_10: width = 1920, height = 1440; break;
	case Resolution::xl16_10: width = 2560, height = 1600; break;
	case Resolution::s16_9: width = 1024, height = 567; break;
	case Resolution::m16_9: width = 1280, height = 720; break;
	case Resolution::sl16_9: width = 1336, height = 768; break;
	case Resolution::ml16_9: width = 1600, height = 900; break;
	case Resolution::l16_9: width = 1920, height = 1080; break;
	case Resolution::xl16_9: width = 2560, height = 1440; break;
	case Resolution::xxl16_9: width = 3480, height = 2160; break;
	}
	return make_pair(width, height);
}

pair<int, int> Window::calculateResolution(Resolution* res) {
	return calculateResolution(*res);
}

Window::~Window()
{
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

Window::Window(string title, int width, int height, Resolution res) {
	resolution = res;
	if (res != Resolution::NONE) {
		auto resolutionDim = calculateResolution(res);
		width = resolutionDim.first;
		height = resolutionDim.second;
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
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        renderer = shared_ptr<Renderer>(new Renderer(window));
		renderer->res = &resolution;
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
	resolution = res;
	if (res != Resolution::NONE) {
		auto resolution = calculateResolution(res);
		width = resolution.first;
		height = resolution.second;
	}
	SDL_SetWindowSize(window, width, height);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

Resolution* Window::getResolution()
{
	return &resolution;
}

void Window::setFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(window, fullscreen);
}

