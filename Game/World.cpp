#include "World.h"
#include <Point.h>
#include <list>
#include <Spritesheet.h>

World::World()
{

}

void World::render()
{
	renderer->renderBackground();
	std::list<Sprite*>::iterator it = sprites.begin();
	while (it != sprites.end()) {
		Sprite* cSprite = *it;
		Point location = cSprite->getLocation();
		//If spritesheet

		//TODO : put this in a sprite/spritesheet render() method, take out isSpritesheet?
		if (cSprite->isSpritesheet) {
			int* box = new int[2];
			((Spritesheet*)cSprite)->getCurrentFrame(box);
			int width = ((Spritesheet*)cSprite)->getWidth();
			int height = ((Spritesheet*)cSprite)->getHeight();
			SDL_Rect dsrect = { location.x, location.y, width, height };
			renderer->render(cSprite->getCurrentImage(),
				{ box[0], box[1], width, height }, dsrect);
			((Spritesheet*)cSprite)->nextFrame();
		}
		else {
			pair<int, int> dimensions = cSprite->getDimensions();
			SDL_Rect dsrect = { location.x, location.y, 
				dimensions.first, dimensions.second };
			renderer->render(cSprite->getCurrentImage(), dsrect);
		}
		it++;
	}
}
list<GameObject*> World::getObjects()
{
	return objects;
}

void World::destroy()
{
	for (list<Sprite*>::iterator i = sprites.begin(); i != sprites.end(); i++)
		(*i)->destroy();
	sprites.clear();
	renderer->destroy();
	delete renderer;
}
