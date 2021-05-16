#include "Sprite.h"
#include "Renderer.h"
#include <stdint.h>
#include <iostream>

Sprite::Sprite()
{
	loopAnimation = false;
	fileBased = false;
	Sprite::surfaces = {};
	initDefaultParams(20);
}

Sprite::Sprite(std::vector<SDL_Surface*> images, int fps, bool loop)
{
	loopAnimation = loop;
	fileBased = false;
	Sprite::surfaces = images;
	initDefaultParams(fps);
}

Sprite::Sprite(std::vector<const char*> filenames, int fps, bool loop)
{
	loopAnimation = loop;
	fileBased = true;
	Sprite::filenames = filenames;
	initDefaultParams(fps);
}

bool Sprite::isOnFinalFrame()
{
	if (std::distance(currentImage, images.end()) == 0) {
		return true;
	}
	return false;
}

//Common elements of the two constructors
void Sprite::initDefaultParams(int fps)
{
	startedAnimation = false;
	paused = false;
	Sprite::fps = fps;
	Sprite::renderTimeBuffer = 1 / (double)(fps);
	Sprite::animationSpeed = ModifiableProperty<double, double>(1);
	Sprite::location = Point(0, 0);
}

std::pair<int, int> Sprite::getDimensions()
{
	if (!loaded || images.size() == 0) {
		return std::make_pair(0, 0);
	}
	SDL_Texture* source = peekCurrentImage();
	int w, h;
	SDL_QueryTexture(source, NULL, NULL, &w, &h);
    return std::make_pair(w, h);
}

SDL_Texture * Sprite::getCurrentImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	if (images.size() == 0) {
		printf("Error: sprite has no images.");
	}
	return *currentImage;
}

void Sprite::tick() {
	//TODO: dont bother with this for non animated sprites
	double renderTimeDiff = (1 / (double)(fps * animationSpeed.getValue())) * 1000;
	if (renderTimeBuffer > renderTimeDiff) {
		nextImage();
		renderTimeBuffer -= renderTimeDiff;
	}
}

void Sprite::nextImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	else if (!paused) {
		if (std::distance(currentImage, images.end()) > 1) {
			currentImage++;
		}
		else {
			currentImage = images.begin();
			if (!loopAnimation) {
				paused = true;
			}
		}
	}
}

SDL_Texture * Sprite::peekCurrentImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	if (loaded && *currentImage) {
		return *currentImage;
	} else {
		printf("Error: no images found in sprite array.");
		return NULL;
	}
}

void Sprite::render(Renderer* renderer, Point locationMod) 
{
	std::pair<int, int> dimensions = getDimensions();
	SDL_Rect dsrect = { (int)location.x + (int)locationMod.x, (int)location.y + (int)locationMod.y, dimensions.first, dimensions.second };
	renderer->render(getCurrentImage(), dsrect);
}

void Sprite::loadTextures(Renderer* renderer) 
{
	if (!loaded) {
		Sprite::loaded = true;
		if (fileBased) {
			for (const char* filename : getFilenames()) {
				SDL_Surface* tempImage = IMG_Load(filename);
				images.push_back(SDL_CreateTextureFromSurface(renderer->getSDLRenderer(), tempImage));
				SDL_FreeSurface(tempImage);
			}
		}
		else {
			for (SDL_Surface* surface : surfaces) {
				images.push_back(SDL_CreateTextureFromSurface(renderer->getSDLRenderer(), surface));
				SDL_FreeSurface(surface);
			}
		}
		std::pair<int, int> cSize = getDimensions();
		size = { cSize.first, cSize.second };
	}
}

void Sprite::destroy()
{
    for (std::list<SDL_Texture*>::iterator i = images.begin(); i != images.end(); i++)
		SDL_DestroyTexture(*i);
	images.clear();
}
