#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <World.h>
#include <Renderer.h>
#include <Point.h>
#include <list>

class Sprite {
public:
	std::list<SDL_Texture*> images;
	Sprite(const char* filename[], World* world) : Sprite(filename, world, 20) {};
	Sprite(const char* filename[], World* world, int fps);
	void setLocation(Point location) {
		Sprite::location = location;
	}
	Point getLocation() {
		return location;
	}
	int getFPS() {
		return fps;
	}
	pair<int, int> getDimensions();
	//Gets current image, advances
	SDL_Texture* getCurrentImage();
	//Does not advance, but starts animation if not started
	SDL_Texture* peekCurrentImage();
	void nextImage();
	void destroy();
	bool isSpritesheet = false;
	double renderTimeBuffer;
private:
	std::list<SDL_Texture*>::iterator currentImage;
	bool startedAnimation;
	Point location;
	int fps;
};