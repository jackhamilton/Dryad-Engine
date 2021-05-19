#include <list>
#include "Scene.h"
#include "Point.h"
#include "Spritesheet.h"
#include "Button.h"
#include "Hitbox.h"

Scene::Scene()
{
	using namespace placeholders;
	sceneMouseMovementEvents = make_shared<vector<pair<Callback, Rectangle>>>(vector<pair<Callback, Rectangle>>());
	sceneMouseEnteredEvents = make_shared<vector<pair<Callback, Rectangle>>>(vector<pair<Callback, Rectangle>>());
	sceneMouseExitedEvents = make_shared<vector<pair<Callback, Rectangle>>>(vector<pair<Callback, Rectangle>>());
	sceneMouseClickEvents = make_shared<vector<pair<Callback, Rectangle>>>(vector<pair<Callback, Rectangle>>());
	sceneMouseRightClickEvents = make_shared<vector<pair<Callback, Rectangle>>>(vector<pair<Callback, Rectangle>>());
	sceneMouseClickUpEvents = make_shared<vector<pair<Callback, Rectangle>>>(vector<pair<Callback, Rectangle>>());
	sceneMouseRightClickUpEvents = make_shared<vector<pair<Callback, Rectangle>>>(vector<pair<Callback, Rectangle>>());
}

std::vector<vector<shared_ptr<GameObject>>> Scene::getObjects()
{
	vector<vector<shared_ptr<GameObject>>> objs;
	auto it = objects.begin();
	for (; it != objects.end(); it++) {
		objs.push_back(it->second);
	}
	return objs;
}

int Scene::getObjectsCount()
{
	int total = 0;
	auto it = objects.begin();
	for (; it != objects.end(); it++) {
		total += it->second.size();
	}
	return total;
}


void Scene::addObject(shared_ptr<GameObject> object) {
	addObject(object, "default");
}

void Scene::addObject(shared_ptr<GameObject> object, string scene) {
	if (!renderer) {
		printf("ERROR: Scene has no renderer. Cannot load sprite textures.");
		return;
	}
	//Load the sprite's images with the scene renderer
	if (object->hasSprite) {
		for (weak_ptr<Sprite> s : object->renderQueue) {
			if (!s.expired()) {
				s.lock()->loadTextures(renderer);
			}
		}
		Scene::objects[scene].push_back(object);
	}

	function<void()> objectMouseEvents[9];
	object->getMouseEvents(objectMouseEvents);

	Rectangle objectSizeRectangle;
	objectSizeRectangle.x = object->getPosition().x;
	objectSizeRectangle.y = object->getPosition().y;
	objectSizeRectangle.width = object->getSize().width;
	objectSizeRectangle.height = object->getSize().height;
	if (object->hasMouseMoveEvent) {
		sceneMouseMovementEvents->push_back(make_pair(objectMouseEvents[0], objectSizeRectangle));
	}
	if (object->hasMouseEnteredEvent) {
		sceneMouseEnteredEvents->push_back(make_pair(objectMouseEvents[1], objectSizeRectangle));
	}
	if (object->hasMouseExitedEvent) {
		sceneMouseExitedEvents->push_back(make_pair(objectMouseEvents[2], objectSizeRectangle));
	}
	if (object->hasMouseClickGraphicEvent) {
		sceneMouseClickEvents->push_back(make_pair(objectMouseEvents[3], objectSizeRectangle));
	}
	if (object->hasMouseClickEvent) {
		sceneMouseClickEvents->push_back(make_pair(objectMouseEvents[4], objectSizeRectangle));
	}
	if (object->hasMouseRightClickEvent) {
		sceneMouseRightClickEvents->push_back(make_pair(objectMouseEvents[5], objectSizeRectangle));
	}
	if (object->hasMouseClickUpGraphicEvent) {
		sceneMouseClickUpEvents->push_back(make_pair(objectMouseEvents[6], objectSizeRectangle));
	}
	if (object->hasMouseClickUpEvent) {
		sceneMouseClickUpEvents->push_back(make_pair(objectMouseEvents[7], objectSizeRectangle));
	}
	if (object->hasMouseRightClickUpEvent) {
		sceneMouseRightClickUpEvents->push_back(make_pair(objectMouseEvents[8], objectSizeRectangle));
	}
	using namespace placeholders;
	object->removeCalls.push_back(bind(&Scene::removeObject, this, _1));
}

void Scene::addSprite(shared_ptr<Sprite> sprite) {
	if (!renderer) {
		printf("ERROR: Scene has no renderer. Cannot load sprite textures.");
		return;
	}
	//Load the sprite's images with the scene renderer
	sprite->loadTextures(renderer);
	sprites.push_back(sprite);
}

void Scene::removeObject(GameObject* o)
{
	auto it = objects.begin();
	for (; it != objects.end(); it++) {
		for (int i = 0; i < objects.size(); i++) {
			if (o->id == it->second.at(i)->id) {
				it->second.erase(it->second.begin() + i);
				i -= 1;
			}
		}
	}
}
