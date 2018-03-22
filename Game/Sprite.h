#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <Window.h>
#include <Renderer.h>
#include <Point.h>
#include <list>

class Sprite {
public:
	std::list<SDL_Texture*> images;
	Sprite(const char* filename[], Window* window);
	void setLocation(Point location) {
		Sprite::location = location;
	}
	Point getLocation() {
		return location;
	}
	pair<int, int> getDimensions();
	//Gets current image, advances
	SDL_Texture* getCurrentImage();
	//Does not advance, but starts animation if not started
	SDL_Texture* peekCurrentImage();
	void destroy();
	bool isSpritesheet = false;
private:
	std::list<SDL_Texture*>::iterator currentImage;
	bool startedAnimation;
	Point location;
};