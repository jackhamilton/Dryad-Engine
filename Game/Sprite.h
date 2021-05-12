#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <list>
#include <vector>
#include "Size.h"
#include "Point.h"
#include "ModifiableProperty.h"

class Renderer;

class Sprite {
public:
	//Blank constructor
	Sprite();
	//Initialize using list of textures
	Sprite(std::vector<SDL_Surface*> images) : Sprite(images, 20, false) {};
	Sprite(std::vector<SDL_Surface*> images, int fps, bool loop);
	//Initialize using image files
    Sprite(std::vector<char*> filenames) : Sprite(filenames, 20, false) {};
    Sprite(std::vector<char*> filenames, int fps, bool loop);

	std::list<SDL_Texture*> images;
	double renderTimeBuffer;
	bool loaded;

    virtual std::pair<int, int> getDimensions();
	//Gets current image, advances
	virtual SDL_Texture* getCurrentImage();
	//Does not advance, but starts animation if not started
	virtual SDL_Texture* peekCurrentImage();
	virtual void nextImage();
	virtual void render(Renderer* renderer, Point locationMod);
	void destroy();
	virtual void loadTextures(Renderer* renderer);
	void tick();

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
	void setAnimationSpeed(double speed) {
		animationSpeed.clearModifiers();
		animationSpeed.addModifier(speed);
	}
	void setPaused(bool pause) {
		Sprite::paused = pause;
	}
private:
	std::list<SDL_Texture*>::iterator currentImage;
	bool startedAnimation;
	Point location;
	Size size;
	int fps;
	bool loopAnimation;
	ModifiableProperty animationSpeed;
	double animationSpeedMultiplier;
	//True if sprite uses files, false if sprite uses a textureset
	bool fileBased;
	std::vector<char*> filenames;
	std::vector<SDL_Surface*> surfaces;
protected:
	void initDefaultParams(int fps);
	bool paused;
};
