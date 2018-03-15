#pragma once


enum Resolution {
	//This is most of the standard screen resolutions for various aspect ratios. 
	//xxs = extra extra small, ml means medium large (smaller than large), etc.
	/*
	xxs4_3 = (640, 480), xs4_3 = (800, 600), s4_3 = (1024, 768),
	m4_3 = (1280, 960), sl4_3 = (1440, 1080), ml4_3 = (1600, 1200), l4_3 = (1920, 1440),

	s16_10 = (1280, 800), m16_10 = (1440, 900), sl16_10 = (1680, 1050),
	l16_10 = (1920, 1200), xl16_10 = (2560, 1600),

	s16_9 = (1024, 567), m16_9 = (1280, 720), sl16_9 = (1336, 768),
	ml16_9 = (1600, 900), l16_9 = (1920, 1080), xl16_9 = (2560, 1440), xxl16_9 = (3480, 2160)*/

	xxs4_3, xs4_3, s4_3, m4_3, sl4_3, ml4_3, l4_3,

	s16_10, m16_10, sl16_10, l16_10, xl16_10,

	s16_9, m16_9, sl16_9, ml16_9, l16_9, xl16_9, xxl16_9

};

class Window {
public:
	int screenWidth;
	int screenHeight;
	//Resolution on default constructor overrides width and height, 
	//there because it has to be and constructors cant be overloaded
	Window(const char *title, int width, int height, Resolution res);
	Window(const char *title, Resolution resolution) : Window(title, 0, 0, resolution) {}
	Window() : Window("", 640, 480, (Resolution)NULL) {};
	void changeWindowResolution(Resolution res);
	void changeWindowResolution(int width, int height);
	void changeWindowResolution(Resolution res, int width, int height);
	void destroy();
private:
	SDL_Window* window;
	void calculateResolution(Resolution res, int &width, int &height);
};