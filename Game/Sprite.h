#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Renderer.h"
#include "Point.h"
#include <list>
#include <vector>

class Sprite {
public:
	//Initialize using list of textures
	Sprite(std::vector<SDL_Texture*> textures) : Sprite(textures, 20) {};
	Sprite(std::vector<SDL_Texture*> textures, int fps);
	//Initialize using image files
    Sprite(std::vector<char*> filenames) : Sprite(filenames, 20) {};
    Sprite(std::vector<char*> filenames, int fps);

	std::list<SDL_Texture*> images;
	bool isSpritesheet = false;
	double renderTimeBuffer;
	//True if sprite uses files, false if sprite uses a textureset
	bool fileBased;

    std::pair<int, int> getDimensions();
	//Gets current image, advances
	SDL_Texture* getCurrentImage();
	//Does not advance, but starts animation if not started
	SDL_Texture* peekCurrentImage();
	void nextImage();
	void destroy();

	std::vector<char*> getFilenames() {
		return filenames;
	}
	void setLocation(Point location) {
		Sprite::location = location;
	}
	Point getLocation() {
		return location;
	}
	int getFPS() {
		return fps;
	}
private:
	std::list<SDL_Texture*>::iterator currentImage;
	bool startedAnimation;
	Point location;
	int fps;
	std::vector<char*> filenames;

	void initDefaultParams(int fps);
};
