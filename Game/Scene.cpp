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
		//If spritesheet

		//TODO : put this in a sprite/spritesheet render() method, take out isSpritesheet?
		if (cSprite->isSpritesheet) {
			int* box = new int[2];
			((Spritesheet*)cSprite)->getCurrentFrame(box);
			int width = ((Spritesheet*)cSprite)->getWidth();
			int height = ((Spritesheet*)cSprite)->getHeight();
			SDL_Rect dsrect = { (int)location.x, (int)location.y, width, height };
			renderer->render(cSprite->getCurrentImage(),
				{ box[0], box[1], width, height }, dsrect);
		}
		else {
			std::pair<int, int> dimensions = cSprite->getDimensions();
			SDL_Rect dsrect = { (int)location.x, (int)location.y,
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
	}
	//Do the same thing but for GameObjects
	for (GameObject* obj: objects) {
		Sprite* cSprite = obj->getSprite();
		cSprite->renderTimeBuffer += MSPerFrame;
		Point location = cSprite->getLocation();
		//If spritesheet

		//TODO : put this in a sprite/spritesheet render() method, take out isSpritesheet?
		Point objLocation = obj->getPosition();
		if (cSprite->isSpritesheet) {
			int* box = new int[2];
			((Spritesheet*)cSprite)->getCurrentFrame(box);
			int width = ((Spritesheet*)cSprite)->getWidth();
			int height = ((Spritesheet*)cSprite)->getHeight();
			SDL_Rect dsrect = { (int)location.x + (int)objLocation.x, (int)location.y + (int)objLocation.y, width, height };
			renderer->render(cSprite->getCurrentImage(),
				{ box[0], box[1], width, height }, dsrect);
		}
		else {
			std::pair<int, int> dimensions = cSprite->getDimensions();
			SDL_Rect dsrect = { (int)location.x + (int)objLocation.x, (int)location.y + (int)objLocation.y,
				dimensions.first, dimensions.second };
			renderer->render(cSprite->getCurrentImage(), dsrect);
		}
		if (cSprite->renderTimeBuffer > (1 / (double)(cSprite->getFPS())) * 1000) {

			if (cSprite->isSpritesheet) {
				((Spritesheet*)cSprite)->nextFrame();
			}
			else {
				cSprite->nextImage();
			}
			cSprite->renderTimeBuffer -= (1 / (double)(cSprite->getFPS())) * 1000;
		}
	}
}
std::vector<GameObject*> Scene::getObjects()
{
	return objects;
}

void Scene::addObject(GameObject* object) {
	//Load the sprite's images with the scene renderer
	Sprite* sprite = object->getSprite();
	if (sprite->fileBased) {
		for (char* filename : sprite->getFilenames()) {
			SDL_Surface * tempImage = IMG_Load(filename);
			sprite->images.push_back(SDL_CreateTextureFromSurface(
				renderer->getSDLRenderer(), tempImage));
			SDL_FreeSurface(tempImage);
		}
	} else {
		printf("Texture loading functionality not implemented (Scene.cpp:99)");
	}
	Scene::objects.push_back(object);
}

void Scene::addSprite(Sprite* sprite) {
	//Load the sprite's images with the scene renderer
	if (sprite->fileBased) {
		for (char* filename : sprite->getFilenames()) {
			SDL_Surface * tempImage = IMG_Load(filename);
			sprite->images.push_back(SDL_CreateTextureFromSurface(
				renderer->getSDLRenderer(), tempImage));
			SDL_FreeSurface(tempImage);
		}
	} else {
		printf("Texture loading functionality not implemented (Scene.cpp:114)");
	}
	sprites.push_back(sprite);
}

void Scene::destroy()
{
    for (Sprite* s: sprites)
		s->destroy();
	sprites.clear();
	renderer->destroy();
}
