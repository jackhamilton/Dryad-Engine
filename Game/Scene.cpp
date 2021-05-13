#include <list>
#include "Scene.h"
#include "Point.h"
#include "Spritesheet.h"
#include "Button.h"
#include "Hitbox.h"

Scene::Scene()
{
	using namespace placeholders;
	Scene::movementCallback = bind(&Scene::moveObject, this, _1, _2);
}

std::vector<GameObject*> Scene::getObjects()
{
	return objects;
}

void Scene::addObject(GameObject* object) {
	if (!renderer) {
		printf("ERROR: Scene has no renderer. Cannot load sprite textures.");
		return;
	}
	//Load the sprite's images with the scene renderer
	if (object->hasSprite) {
		for (Sprite* s : object->renderQueue) {
			s->loadTextures(renderer);
		}
		Scene::objects.push_back(object);
	}

	object->movementCallback = &movementCallback;

	function<void()> objectMouseEvents[9];
	object->getMouseEvents(objectMouseEvents);

	Rectangle objectSizeRectangle;
	objectSizeRectangle.x = object->getPosition().x;
	objectSizeRectangle.y = object->getPosition().y;
	objectSizeRectangle.width = object->getSize().width;
	objectSizeRectangle.height = object->getSize().height;
	if (object->hasMouseMoveEvent) {
		sceneMouseMovementEvents.push_back(make_pair(objectMouseEvents[0], objectSizeRectangle));
	}
	if (object->hasMouseEnteredEvent) {
		sceneMouseEnteredEvents.push_back(make_pair(objectMouseEvents[1], objectSizeRectangle));
	}
	if (object->hasMouseExitedEvent) {
		sceneMouseExitedEvents.push_back(make_pair(objectMouseEvents[2], objectSizeRectangle));
	}
	if (object->hasMouseClickGraphicEvent) {
		sceneMouseClickEvents.push_back(make_pair(objectMouseEvents[3], objectSizeRectangle));
	}
	if (object->hasMouseClickEvent) {
		sceneMouseClickEvents.push_back(make_pair(objectMouseEvents[4], objectSizeRectangle));
	}
	if (object->hasMouseRightClickEvent) {
		sceneMouseRightClickEvents.push_back(make_pair(objectMouseEvents[5], objectSizeRectangle));
	}
	if (object->hasMouseClickUpGraphicEvent) {
		sceneMouseClickUpEvents.push_back(make_pair(objectMouseEvents[6], objectSizeRectangle));
	}
	if (object->hasMouseClickUpEvent) {
		sceneMouseClickUpEvents.push_back(make_pair(objectMouseEvents[7], objectSizeRectangle));
	}
	if (object->hasMouseRightClickUpEvent) {
		sceneMouseRightClickUpEvents.push_back(make_pair(objectMouseEvents[8], objectSizeRectangle));
	}
}

void Scene::addSprite(Sprite* sprite) {
	if (!renderer) {
		printf("ERROR: Scene has no renderer. Cannot load sprite textures.");
		return;
	}
	//Load the sprite's images with the scene renderer
	sprite->loadTextures(renderer);
	sprites.push_back(sprite);
}

//Speed can be affected by modifiers, including frame speed. Vector is px/sec. Set position for absolute movement.
void Scene::moveObject(GameObject* g, ModifiableProperty<Vector, double> vector)
{
	std::vector<Hitbox*> hitboxes;
	for (GameObject* o : objects) {
		if (!g->id == o->id && o->hitboxEnabled && o->hitbox) {
			hitboxes.push_back(o->hitbox);
		}
	}
	double fpsSpeedFactor;
	if (lastFrameTimeMS) {
		fpsSpeedFactor = **lastFrameTimeMS / 1000;
	}
	else if (defaultFps) {
		fpsSpeedFactor = 1.0 / (double)(**defaultFps);
	}
	else {
		fpsSpeedFactor = 1.0 / 60.0;
	}
	vector.addModifier(fpsSpeedFactor);
	Vector v = g->hitbox->getMaximumClearDistanceForVectorFromGameObject(hitboxes, vector.getValue());
	Point adjustedPosition = g->position + v.getCartesian();
	g->setPosition(adjustedPosition);
}

void Scene::destroy()
{
    for (Sprite* s: sprites)
		s->destroy();
	sprites.clear();
	renderer->destroy();
}
