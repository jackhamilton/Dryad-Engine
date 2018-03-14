#pragma once

class Window {
public:
	int screenWidth;
	int screenHeight;
	Window(const char *title, int width, int height);
	Window() : Window("", 640, 480) {};
	void destroy();
private:
	SDL_Window* window;
};