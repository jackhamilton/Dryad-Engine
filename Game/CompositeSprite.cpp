#include "CompositeSprite.h"
using namespace std;

CompositeSprite::CompositeSprite(int fps)
{
	initDefaultParams(20);
}

void CompositeSprite::addSpriteFromSurfaces(vector<SDL_Surface*> surfaces)
{
	if (imageSurfaceStack.size() == 0 ||
		(imageSurfaceStack.size() > 0 && surfaces.size() == imageSurfaceStack.front().size())) {
		imageSurfaceStack.push_back(surfaces);
	}
	else {
		printf("Error: attempted to add surfaces with a different framecount than existing surfaces in a CompositeSprite.");
	}
}

void CompositeSprite::addSpriteFromFiles(vector<char*> files)
{
	if (imageFileStack.size() == 0 ||
		(imageFileStack.size() > 0 && files.size() == imageFileStack.front().size())) {
		imageFileStack.push_back(files);
	}
	else {
		printf("Error: attempted to add files with a different framecount than existing files in a CompositeSprite.");
	}
}

void CompositeSprite::loadTextures(Renderer* renderer)
{
	for (vector<char*> list : imageFileStack) {
		std::list<SDL_Surface*> tmp;
		for (char* file : list) {
			SDL_Surface* tempImage = IMG_Load(file);
			tmp.push_back(tempImage);
		}
		std::vector<SDL_Surface*> v{ std::begin(tmp), std::end(tmp) };
		imageSurfaceStack.push_back(v);
	}
	if (imageSurfaceStack.size() > 0) {
		int i = 0;
		while (i < imageSurfaceStack.front().size()) {
			std::list<SDL_Surface*> blitTogether;
			std::list<std::vector<SDL_Surface*>>::iterator iterate = imageSurfaceStack.begin();
			while (iterate != imageSurfaceStack.end()) {
				blitTogether.push_back((*iterate).at(i));
				iterate++;
			}
			std::list<SDL_Surface*>::iterator iterator = blitTogether.begin();
			SDL_Surface* dest = *iterator;
			iterator++;
			while (iterator != blitTogether.end()) {
				//TODO: Offsets
				SDL_BlitSurface(*iterator, NULL, dest, NULL);
				iterator++;
			}
			images.push_back(SDL_CreateTextureFromSurface(renderer->getSDLRenderer(), dest));
			for (SDL_Surface* surface : blitTogether) {
				SDL_FreeSurface(surface);
			}
			i++;
		}
	}
}

CompositeSprite::~CompositeSprite()
{
}
