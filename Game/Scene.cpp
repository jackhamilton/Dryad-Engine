#include <list>
#include "Scene.h"
#include "Point.h"
#include "Spritesheet.h"
#include "TextField.h"
#include "Button.h"
#include "Hitbox.h"
#include <memory>

Scene::Scene()
{
	using namespace placeholders;
}

std::vector<vector<shared_ptr<GameObject>>> Scene::getObjects()
{
	vector<vector<shared_ptr<GameObject>>> objs;
	if (objects.size() > 0) {
		auto it = objects.begin();
		for (; it != objects.end(); it++) {
			objs.push_back(it->second);
		}
	}
	return objs;
}

std::vector<shared_ptr<GameObject>> Scene::getObjectsFlat()
{
	vector<shared_ptr<GameObject>> objs;
	auto it = objects.begin();
	for (; it != objects.end(); it++) {
		for (shared_ptr<GameObject> o : it->second) {
			objs.push_back(o);
		}
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

void Scene::addObject(shared_ptr<GameObject> object, string layer) {
	if (input && input->lockSceneObjects) {
		postInputLoopObjectAddQueue.push_back(make_pair(object, layer));
	}
	else {
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
			Scene::objects[layer].push_back(object);
		}

		function<void()> objectMouseEvents[9];
		object->getMouseEvents(objectMouseEvents);

		if (object->isTextField) {
			using namespace placeholders;
			static_pointer_cast<TextField>(object)->deactivateOtherCallback = bind(&Scene::deactivateUniqueElements, this, _1);
			static_pointer_cast<TextField>(object)->activateFieldCallback = bind(&Scene::activateTextField, this, _1);
		}

		using namespace placeholders;
		object->removeCalls.push_back(bind(&Scene::removeObject, this, _1));
	}
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
	if (input && input->lockSceneObjects) {
		postInputLoopObjectRemovalQueue.push_back(o);
	}
	else {
		auto it = objects.begin();
		for (; it != objects.end(); it++) {
			for (int i = 0; i < it->second.size(); i++) {
				if (o->id == it->second.at(i)->id) {
					it->second.erase(it->second.begin() + i);
					i -= 1;
				}
			}
		}
	}
}

void Scene::handleObjectModificationQueue() {
	for (GameObject* o : postInputLoopObjectRemovalQueue) {
		removeObject(o);
	}
	for (pair<shared_ptr<GameObject>, string> o : postInputLoopObjectAddQueue) {
		addObject(o.first, o.second);
	}
	postInputLoopObjectRemovalQueue.clear();
	postInputLoopObjectAddQueue.clear();
}

void Scene::deactivateUniqueElements(GameObject* sender)
{
	if (sender->isTextField) {
		for (shared_ptr<GameObject> g : getObjectsFlat()) {
			if (g->isTextField) {
				auto tf = static_pointer_cast<TextField>(g);
				tf->deactivate();
			}
		}
	}
}

void Scene::activateTextField(TextField* object) {
	input->activeField = object;
}
