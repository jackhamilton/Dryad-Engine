#include "Sprite.h"
#include <stdint.h>
#include <iostream>

Sprite::Sprite(std::vector<char*> filenames, int fps)
{
	Sprite::filenames = filenames;
	startedAnimation = false;
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
	if (images.size() == 0) {
		printf("Error: sprite has no images.");
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
