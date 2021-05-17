#include "CompositeSprite.h"
using namespace std;

CompositeSprite::CompositeSprite(int fps)
{
	initDefaultParams(20);
	centerAll = false;
}

void CompositeSprite::addSpriteFromSurfaces(vector<SDL_Surface*> surfaces, Point offset)
{
	if (imageSurfaceStack.size() == 0 ||
		(imageSurfaceStack.size() > 0 && surfaces.size() == imageSurfaceStack.front().size())) {
		imageSurfaceStack.push_back(surfaces);
		loaded = false;
	}
	else {
		printf("Error: attempted to add surfaces with a different framecount than existing surfaces in a CompositeSprite.");
	}
}

void CompositeSprite::addSpriteFromSurfaces(vector<SDL_Surface*> surfaces)
{
	addSpriteFromSurfaces(surfaces, { 0,0 });
}

void CompositeSprite::addSpriteFromFiles(vector<string> files, Point offset)
{
	if (imageFileStack.size() == 0 ||
		(imageFileStack.size() > 0 && files.size() == imageFileStack.front().size())) {
		imageFileStack.push_back(files);
		loaded = false;
	}
	else {
		printf("Error: attempted to add files with a different framecount than existing files in a CompositeSprite.");
	}
}

void CompositeSprite::addSpriteFromFiles(vector<string> files)
{
	addSpriteFromFiles(files, { 0,0 });
}

void CompositeSprite::loadTextures(shared_ptr<Renderer> renderer)
{
	if (!loaded) {
		Sprite::loaded = true;
		vector<Point> offsets;
		int iterator = 0;
		//DESTROYS imageFileStack and fileOffsets
		for (vector<string> list : imageFileStack) {
			std::list<SDL_Surface*> tmp;
			for (string file : list) {
				SDL_Surface* tempImage = IMG_Load(file.c_str());
				tmp.push_back(tempImage);
			}
			vector<SDL_Surface*> v{ begin(tmp), end(tmp) };
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
				list<SDL_Surface*> blitTogether;
				auto iterate = imageSurfaceStack.begin();
				while (iterate != imageSurfaceStack.end()) {
					blitTogether.push_back((*iterate).at(i));
					iterate++;
				}
				list<SDL_Surface*>::iterator iterator = blitTogether.begin();
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
						SDL_BlitSurface((*iterator), NULL, dest, &dstrect);
					}
					else {
						Point cOffset = offsets.front();
						offsets.erase(offsets.begin());
						SDL_Rect dstrect = SDL_Rect();
						dstrect.x = cOffset.x;
						dstrect.y = cOffset.y;
						dstrect.w = (*iterator)->w;
						dstrect.h = (*iterator)->h;
						SDL_BlitSurface((*iterator), NULL, dest, &dstrect);
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
		std::pair<int, int> cSize = getDimensions();
		size = { cSize.first, cSize.second };
	}
}

void CompositeSprite::setCenterAll(bool centered) {
	centerAll = centered;
}