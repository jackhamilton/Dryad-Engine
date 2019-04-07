#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <list>
#include <vector>
#include "Point.h"

class Renderer;

class Sprite {
public:
	//Blank constructor
	Sprite();
	//Initialize using list of textures
	Sprite(std::vector<SDL_Surface*> images) : Sprite(images, 20) {};
	Sprite(std::vector<SDL_Surface*> images, int fps);
	//Initialize using image files
    Sprite(std::vector<char*> filenames) : Sprite(filenames, 20) {};
    Sprite(std::vector<char*> filenames, int fps);

	std::list<SDL_Texture*> images;
	double renderTimeBuffer;

    virtual std::pair<int, int> getDimensions();
	//Gets current image, advances
	virtual SDL_Texture* getCurrentImage();
	//Does not advance, but starts animation if not started
	virtual SDL_Texture* peekCurrentImage();
	virtual void nextImage();
	virtual void render(Renderer* renderer, Point locationMod);
	void destroy();
	virtual void loadTextures(Renderer* renderer);

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
	//True if sprite uses files, false if sprite uses a textureset
	bool fileBased;
	std::vector<char*> filenames;
	std::vector<SDL_Surface*> surfaces;
protected:
	void initDefaultParams(int fps);
};
