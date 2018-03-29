#include "Sprite.h"
#include <stdint.h>
#include <iostream>

Sprite::Sprite(std::vector<char*> filenames, Scene* world, int fps)
{
	startedAnimation = false;
	(*world).sprites.push_back(this);
	Renderer* windowRenderer = world->getRenderer();
	Renderer renderer = *windowRenderer;
    for (char* filename : filenames) {
		SDL_Surface * tempImage = IMG_Load(filename);
		Sprite::images.push_back(SDL_CreateTextureFromSurface(//Maybe move this to the render loop
			renderer.getSDLRenderer(), tempImage));
		SDL_FreeSurface(tempImage);
	}
	Sprite::fps = fps;
	isSpritesheet = false;
	Sprite::renderTimeBuffer = 1 / (double)(fps);
}

std::pair<int, int> Sprite::getDimensions()
{
	SDL_Texture* source = peekCurrentImage();
	int w, h;
	SDL_QueryTexture(source, NULL, NULL, &w, &h);
    return std::pair<int, int>(w, h);
}

SDL_Texture * Sprite::getCurrentImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	return *currentImage;
}

void Sprite::nextImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	else {
		if (std::distance(currentImage, images.end()) > 1) {
			currentImage++;
		}
		else {
			currentImage = images.begin();
		}
	}
}

SDL_Texture * Sprite::peekCurrentImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	try {
		return *currentImage;
	}
    catch (std::exception e) {
		printf("Error: no images found in sprite array.");
		return NULL;
	}
}

void Sprite::destroy()
{
    for (std::list<SDL_Texture*>::iterator i = images.begin(); i != images.end(); i++)
		SDL_DestroyTexture(*i);
	images.clear();
}
