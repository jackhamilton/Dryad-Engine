#include "Sprite.h"
#include "Renderer.h"
#include <stdint.h>
#include <iostream>
#include <queue>

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

Sprite::Sprite(std::vector<string> filenames, int fps, bool loop)
{
	loopAnimation = loop;
	fileBased = true;
	Sprite::filenames = filenames;
	initDefaultParams(fps);
}

//Common elements of the two constructors
void Sprite::initDefaultParams(int fps)
{
	loaded = false;
	startedAnimation = false;
	paused = false;
	Sprite::fps = fps;
	Sprite::renderTimeBuffer = 1 / (double)(fps);
	Sprite::animationSpeed = ModifiableProperty<double, double>(1);
	Sprite::location = Point(0, 0);
}

Sprite::~Sprite()
{
	//IF memory leak, check renderQueue, compositeSprite render stack
	int imagesCount = images.size();
	std::queue<SDL_Texture*> texturesToDestroy;
	for (SDL_Texture* image : images) {
		texturesToDestroy.push(image);
	}
	while (!texturesToDestroy.empty()) {
		SDL_Texture* tex = texturesToDestroy.front();
		texturesToDestroy.pop();
		if (tex) {
			SDL_DestroyTexture(tex);
		}
	}
	imagesCount = surfaces.size();
	std::queue<SDL_Surface*> surfacesToDestroy;
	for (SDL_Surface* image : surfaces) {
		surfacesToDestroy.push(image);
	}
	while (!surfacesToDestroy.empty()) {
		SDL_Surface* surf = surfacesToDestroy.front();
		surfacesToDestroy.pop();
		if (surf) {
			SDL_FreeSurface(surf);
		}
	}
}

bool Sprite::isOnFinalFrame()
{
	if (distance(currentImage, images.end()) == 0) {
		return true;
	}
	return false;
}

pair<int, int> Sprite::getDimensions()
{
	if (!loaded || images.size() == 0) {
		return make_pair(0, 0);
	}
	SDL_Texture* source = peekCurrentImage();
	int w, h;
	SDL_QueryTexture(source, NULL, NULL, &w, &h);
    return make_pair(w, h);
}

SDL_Texture* Sprite::getCurrentImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	if (images.size() == 0) {
		printf("Error: sprite has no images.");
		return nullptr;
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

SDL_Texture* Sprite::peekCurrentImage()
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

void Sprite::render(shared_ptr<Renderer> renderer, Point locationMod)
{
	pair<int, int> dimensions = getDimensions();
	SDL_Rect dsrect = { (int)location.x + (int)locationMod.x, (int)location.y + (int)locationMod.y, dimensions.first, dimensions.second };
	renderer->render(getCurrentImage(), dsrect);
}

void Sprite::loadTextures(shared_ptr<Renderer> renderer)
{
	if (!loaded) {
		Sprite::loaded = true;
		if (fileBased) {
			for (string filename : getFilenames()) {
				SDL_Surface* tempImage = IMG_Load(filename.c_str());
				images.push_back(SDL_CreateTextureFromSurface(renderer->getSDLRenderer(), tempImage));
				SDL_FreeSurface(tempImage);
			}
		}
		else {
			auto it = surfaces.begin();
			for (it; it < surfaces.end(); it++) {
				images.push_back(SDL_CreateTextureFromSurface(renderer->getSDLRenderer(), (*it)));
				SDL_Surface* surf = *it;
				SDL_FreeSurface(surf);
			}
			surfaces.clear();
		}
		pair<int, int> cSize = getDimensions();
		size = { cSize.first, cSize.second };
	}
}