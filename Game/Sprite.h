#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Renderer.h"
#include "Point.h"
#include <list>
#include <vector>

class Sprite {
public:
	std::list<SDL_Texture*> images;
    Sprite(std::vector<char*> filenames) : Sprite(filenames, 20) {};
    Sprite(std::vector<char*> filenames, int fps);
	void setLocation(Point location) {
		Sprite::location = location;
	}
	Point getLocation() {
		return location;
	}
	int getFPS() {
		return fps;
	}
    std::pair<int, int> getDimensions();
	//Gets current image, advances
	SDL_Texture* getCurrentImage();
	//Does not advance, but starts animation if not started
	SDL_Texture* peekCurrentImage();
	void nextImage();
	void destroy();
	bool isSpritesheet = false;
	double renderTimeBuffer;
	std::vector<char*> getFilenames() {
		return filenames;
	}
private:
	std::list<SDL_Texture*>::iterator currentImage;
	bool startedAnimation;
	Point location;
	int fps;
	std::vector<char*> filenames;
};
