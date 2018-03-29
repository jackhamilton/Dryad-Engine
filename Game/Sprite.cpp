#include <Sprite.h>
#include <stdint.h>
#include <iostream>

Sprite::Sprite(const char* filename[], World* world)
{
	startedAnimation = false;
	(*world).sprites.push_back(this);
	Renderer* windowRenderer = world->getRenderer();
	Renderer renderer = *windowRenderer;
	for (int x = 0; x < sizeof(filename)/sizeof(filename[0]); x++) {
		SDL_Surface * tempImage = IMG_Load(filename[x]);
		Sprite::images.push_back(SDL_CreateTextureFromSurface(//Maybe move this to the render loop
			renderer.getSDLRenderer(), tempImage));
		SDL_FreeSurface(tempImage); 
		//might cause problems? Freeing the surface before rendering
	}
	isSpritesheet = false;
}

pair<int, int> Sprite::getDimensions()
{
	SDL_Texture* source = peekCurrentImage();
	int w, h;
	SDL_QueryTexture(source, NULL, NULL, &w, &h);
	return pair<int, int>(w, h);
}

SDL_Texture * Sprite::getCurrentImage()
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
	return *currentImage;
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
	catch (exception e) {
		printf("Error: no images found in sprite array.");
		return NULL;
	}
}

void Sprite::destroy()
{
	for (list<SDL_Texture*>::iterator i = images.begin(); i != images.end(); i++)
		SDL_DestroyTexture(*i);
	images.clear();
}
