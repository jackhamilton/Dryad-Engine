#include "CompositeSprite.h"

#include <queue>

namespace dryad
{

CompositeSprite::CompositeSprite(int fps)
{
	initDefaultParams(20);
	centerAll = false;
}

void CompositeSprite::addSpriteFromSurfaces(std::vector<SDL_Surface*> surfaces, Point offset, int clip)
{
	if (imageSurfaceStack.size() == 0 ||
		(imageSurfaceStack.size() > 0 && surfaces.size() == imageSurfaceStack.front().size())) {
		imageSurfaceStack.push_back(surfaces);
		surfaceOffsets.push_back(std::make_pair(offset, clip));
		loaded = false;
	}
	else {
		printf("Error: attempted to add surfaces with a different framecount than existing surfaces in a CompositeSprite.");
	}
}

void CompositeSprite::addSpriteFromSurfaces(std::vector<SDL_Surface*> surfaces)
{
	addSpriteFromSurfaces(surfaces, { 0, 0 }, 0);
}

void CompositeSprite::addSpriteFromFiles(std::vector<std::string> files, Point offset, int clip)
{
	if (imageFileStack.size() == 0 ||
		(imageFileStack.size() > 0 && files.size() == imageFileStack.front().size())) {
		imageFileStack.push_back(files);
		fileOffsets.push_back(std::make_pair(offset, clip));
		loaded = false;
	}
	else {
		printf("Error: attempted to add files with a different framecount than existing files in a CompositeSprite.");
	}
}

void CompositeSprite::addSpriteFromFiles(std::vector<std::string> files)
{
	addSpriteFromFiles(files, { 0,0 }, 0);
}

void CompositeSprite::loadTextures(std::shared_ptr<Renderer> renderer)
{
	if (!loaded) {
		Sprite::loaded = true;
		std::vector<std::pair<Point, int>> offsets;
		int iterator = 0;
		//DESTROYS imageFileStack and fileOffsets
		for (std::vector<std::string> list : imageFileStack) {
			std::list<SDL_Surface*> tmp;
			for (std::string file : list) {
				SDL_Surface* tempImage = IMG_Load(file.c_str());
				tmp.push_back(tempImage);
			}
			std::vector<SDL_Surface*> v{ begin(tmp), end(tmp) };
			imageSurfaceStack.push_back(v);
			offsets.push_back(fileOffsets.front());
			fileOffsets.erase(fileOffsets.begin());
		}
		imageFileStack.clear();
		for (std::pair<Point, int> surfaceOffset : surfaceOffsets) {
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
				offsets.erase(offsets.begin());
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
						Point cOffset = offsets.front().first;
						int clip = offsets.front().second;
						offsets.erase(offsets.begin());
						SDL_Rect dstrect = SDL_Rect();
						dstrect.x = cOffset.x;
						dstrect.y = cOffset.y;
						if (cOffset.x + (*iterator)->w + clip > dest->w) {
							dstrect.w = cOffset.x > dest->w ? 0 : dest->w - cOffset.x - clip;
						}
						else {
							dstrect.w = (*iterator)->w;
						}
						if (cOffset.y + (*iterator)->h + clip > dest->h) {
							dstrect.h = cOffset.y > dest->h ? 0 : dest->h - cOffset.y - clip;
						}
						else {
							dstrect.h = (*iterator)->h;
						}
						SDL_Rect srcrect = SDL_Rect();
						srcrect.x = 0;
						srcrect.y = 0;
						srcrect.w = dstrect.w;
						srcrect.h = dstrect.h;
						SDL_BlitSurface((*iterator), &srcrect, dest, &dstrect);
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
		imageSurfaceStack.clear();
		imageFileStack.clear();
		surfaceOffsets.clear();
		fileOffsets.clear();
		if (size.width == 0 && size.height == 0) {
			std::pair<int, int> cSize = getDimensions();
			size = { cSize.first, cSize.second };
		}
		currentImage = images.begin();
	}
}

void CompositeSprite::reloadTopTextureFromStack() {
	imageSurfaceStack.clear();
	imageFileStack.clear();
	surfaceOffsets.clear();
	fileOffsets.clear();
	SDL_Texture* tex = images.back();
	if (tex) {
		SDL_DestroyTexture(tex);
		images.pop_back();
	}
	loaded = false;
}

void CompositeSprite::reloadAllTextures() {
	imageSurfaceStack.clear();
	imageFileStack.clear();
	surfaceOffsets.clear();
	fileOffsets.clear();
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
	images.clear();
	loaded = false;
}

void CompositeSprite::setCenterAll(bool centered) {
	centerAll = centered;
}

}