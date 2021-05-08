#include "Scene.h"
#include "Point.h"
#include <list>
#include "Spritesheet.h"

Scene::Scene()
{
    
}

void Scene::render(int frame, int fps)
{
	double MSPerFrame = (1 / (double)fps) * 1000;
	renderer->renderBackground();;
	for (Sprite* cSprite: sprites) {
		cSprite->renderTimeBuffer += MSPerFrame;
		Point location = cSprite->getLocation();
		
		cSprite->render(renderer, Point(0, 0));

		cSprite->tick();
	}
	//Do the same thing but for GameObjects
	for (GameObject* obj: objects) {
		if (obj->hasSprite) {
			Sprite* cSprite = obj->getSprite();
			cSprite->renderTimeBuffer += MSPerFrame;
			Point location = cSprite->getLocation();
			//If spritesheet

			cSprite->render(renderer, obj->getPosition());
			
			cSprite->tick();
		}
	}
}
std::vector<GameObject*> Scene::getObjects()
{
	return objects;
}

void Scene::addObject(GameObject* object) {
	//Load the sprite's images with the scene renderer
	if (object->hasSprite) {
		Sprite* sprite = object->getSprite();
		sprite->loadTextures(renderer);
		Scene::objects.push_back(object);
	}
}

void Scene::addSprite(Sprite* sprite) {
	//Load the sprite's images with the scene renderer
	sprite->loadTextures(renderer);
	sprites.push_back(sprite);
}

void Scene::destroy()
{
    for (Sprite* s: sprites)
		s->destroy();
	sprites.clear();
	renderer->destroy();
}
