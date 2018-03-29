#include "Scene.h"
#include <Point.h>
#include <list>
#include <Spritesheet.h>

Scene::Scene()
{

}

void Scene::render(int frame, int fps)
{
	double MSPerFrame = (1 / (double)fps) * 1000;
	renderer->renderBackground();
	std::list<Sprite*>::iterator it = sprites.begin();
	while (it != sprites.end()) {
		Sprite* cSprite = *it;
		cSprite->renderTimeBuffer += MSPerFrame;
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
		}
		else {
			pair<int, int> dimensions = cSprite->getDimensions();
			SDL_Rect dsrect = { location.x, location.y,
				dimensions.first, dimensions.second };
			renderer->render(cSprite->getCurrentImage(), dsrect);
		}
		if (cSprite->renderTimeBuffer > (1/(double)(cSprite->getFPS())) * 1000) {

			if (cSprite->isSpritesheet) {
				((Spritesheet*)cSprite)->nextFrame();
			}
			else {
				cSprite->nextImage();
			}
			cSprite->renderTimeBuffer -= (1 / (double)(cSprite->getFPS())) * 1000;
		}
		it++;
	}
}
list<GameObject*> Scene::getObjects()
{
	return objects;
}

void Scene::destroy()
{
	for (list<Sprite*>::iterator i = sprites.begin(); i != sprites.end(); i++)
		(*i)->destroy();
	sprites.clear();
	renderer->destroy();
	delete renderer;
}
