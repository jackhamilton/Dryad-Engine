#include "CompositeSprite.h"
using namespace std;

CompositeSprite::CompositeSprite(int fps)
{
	initDefaultParams(20);
	hasCalledLoadTextures = false;
}

void CompositeSprite::addSpriteFromSurfaces(vector<SDL_Surface*> surfaces, Point offset)
{
	if (imageSurfaceStack.size() == 0 ||
		(imageSurfaceStack.size() > 0 && surfaces.size() == imageSurfaceStack.front().size())) {
		imageSurfaceStack.push_back(surfaces);
	}
	else {
		printf("Error: attempted to add surfaces with a different framecount than existing surfaces in a CompositeSprite.");
	}
}

void CompositeSprite::addSpriteFromSurfaces(vector<SDL_Surface*> surfaces)
{
	addSpriteFromSurfaces(surfaces, { 0,0 });
}

void CompositeSprite::addSpriteFromFiles(vector<char*> files, Point offset)
{
	if (imageFileStack.size() == 0 ||
		(imageFileStack.size() > 0 && files.size() == imageFileStack.front().size())) {
		imageFileStack.push_back(files);
	}
	else {
		printf("Error: attempted to add files with a different framecount than existing files in a CompositeSprite.");
	}
}

void CompositeSprite::addSpriteFromFiles(vector<char*> files)
{
	addSpriteFromFiles(files, { 0,0 });
}

void CompositeSprite::loadTextures(Renderer* renderer)
{
	if (!hasCalledLoadTextures) {
		hasCalledLoadTextures = true;
		vector<Point> offsets;
		int iterator = 0;
		//DESTROYS imageFileStack and fileOffsets
		for (vector<char*> list : imageFileStack) {
			std::list<SDL_Surface*> tmp;
			for (char* file : list) {
				SDL_Surface* tempImage = IMG_Load(file);
				tmp.push_back(tempImage);
			}
			std::vector<SDL_Surface*> v{ std::begin(tmp), std::end(tmp) };
			imageSurfaceStack.push_back(v);
			offsets.push_back(fileOffsets.front());
			fileOffsets.erase(fileOffsets.begin());
		}
		imageFileStack.clear();
		for (Point surfaceOffset : surfaceOffsets) {
			offsets.push_back(surfaceOffset);
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
					if (centerAll) {
						SDL_Rect dstrect = SDL_Rect();
						if ((dest)->w < (*iterator)->w || (dest)->h < (*iterator)->h) {
							printf("DryadCompositeSpriteError: Size of subsurface smaller than size of destination surface.\n");
						}
						dstrect.x = ((dest)->w - (*iterator)->w) / 2;
						dstrect.y = ((dest)->h - (*iterator)->h) / 2;
						dstrect.w = (*iterator)->w;
						dstrect.h = (*iterator)->h;
						SDL_BlitSurface(*iterator, NULL, dest, &dstrect);
					}
					else {
						Point cOffset = offsets.front();
						offsets.erase(offsets.begin());
						SDL_Rect dstrect = SDL_Rect();
						dstrect.x = cOffset.x;
						dstrect.y = cOffset.y;
						dstrect.w = (*iterator)->w;
						dstrect.h = (*iterator)->h;
						SDL_BlitSurface(*iterator, NULL, dest, &dstrect);
					}
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
}

void CompositeSprite::setCenterAll(bool centered) {
	centerAll = centered;
}

CompositeSprite::~CompositeSprite()
{
}
