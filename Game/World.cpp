#include "World.h"
#include <stdio.h>
#include "GameObjectEvent.h"

World::World(shared_ptr<Renderer> renderer, shared_ptr<Input> input)
{
	World::renderer = renderer;
	World::input = input;
	World::mouse = input->getMouse();
}

void World::addScene(shared_ptr<Scene> s, string name)
{
	s->renderer = renderer;
	s->name = name;
	s->lastFrameTimeMS = make_shared<shared_ptr<long double>>(lastFrameTimeMS);
	s->defaultFps = make_shared<shared_ptr<long double>>(defaultFps);
	scenes[name] = s;
	if (!currentScene) {
		setScene(name);
	}
}

void World::setScene(string name)
{
	if (currentScene) {
		for (shared_ptr<GameObject> o : currentScene->objects) {
			o->sceneActive = false;
		}
	}
	if (mouse->sceneMouseExitedEvents) {
		vector<pair<function<void()>, Rectangle>> vec = *mouse->sceneMouseExitedEvents;
		for (vector<pair<function<void()>, Rectangle>>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			pair<function<void()>, Rectangle> pair = *it;
			function<void()> func = pair.first;
			if (func) {
				func();
			}
		}
	}
	if (currentScene) {
		currentScene->isCurrentScene = false;
	}
	currentScene = scenes[name];
	mouse->sceneMouseClickEvents = currentScene->sceneMouseClickEvents;
	mouse->sceneMouseEnteredEvents = currentScene->sceneMouseEnteredEvents;
	mouse->sceneMouseExitedEvents = currentScene->sceneMouseExitedEvents;
	mouse->sceneMouseClickUpEvents = currentScene->sceneMouseClickUpEvents;
	mouse->sceneMouseMovementEvents = currentScene->sceneMouseMovementEvents;
	mouse->sceneMouseRightClickEvents = currentScene->sceneMouseRightClickEvents;
	mouse->sceneMouseRightClickUpEvents = currentScene->sceneMouseRightClickUpEvents;
	mouse->activeScene = true;
	currentScene->isCurrentScene = true;
	for (shared_ptr<GameObject> o : currentScene->objects) {
		o->sceneActive = true;
		o->objects = &(currentScene->objects);
		o->defaultFps = weak_ptr<long double>(defaultFps);
		o->lastFrameTimeMS = weak_ptr<long double>(defaultFps);
	}
}

string World::getCurrentSceneName()
{
	return currentScene->name;
}

shared_ptr<Scene> World::getCurrentScene()
{
	return currentScene;
}

void World::setRenderer(shared_ptr<Renderer> renderer)
{
	World::renderer = renderer;
}

void World::render(int frame, int fps)
{
	if (currentScene) {
		renderer->renderBackground();
		if (frame % 10 == 0) {
			if (displayObjectCount) {
				refreshObjectCount();
			}
		}

		double MSPerFrame = (1 / (double)fps) * 1000;
		for (shared_ptr<Sprite> cSprite : currentScene->sprites) {
			if (!cSprite->loaded) {
				cSprite->loadTextures(renderer);
			}
			cSprite->renderTimeBuffer += MSPerFrame;
			Point location = cSprite->getLocation();
			cSprite->render(renderer, location);
			cSprite->tick();
		}
		//Do the same thing but for GameObjects
		for (shared_ptr<GameObject> obj : currentScene->getObjects()) {
			renderGameObject(obj, Point(0, 0), MSPerFrame);
			obj->handleEvents(currentScene->localTime - currentScene->lastTickTime);
		}
		//And finally for the debug layer
		for (shared_ptr<GameObject> obj : debugObjects) {
			renderGameObject(obj, Point(0, 0), MSPerFrame);
			obj->handleEvents(currentScene->localTime - currentScene->lastTickTime);
		}
		renderer->renderPresent();
		clock_t cTime = clock();
		currentScene->lastTickTime = currentScene->localTime;
		currentScene->localTime += cTime - (lastLocalTime > 0 ? lastLocalTime : cTime);
		lastLocalTime = cTime;
	}
}

void World::renderGameObject(shared_ptr<GameObject> obj, Point positionMod, double MSPerFrame) {
	if (obj->hitboxEnabled && !obj->hitbox) {
		obj->enableHitbox();
	}
	if (displayHitboxes && obj->hitboxEnabled && !obj->hitboxRendered) {
		obj->renderHitbox();
	}
	if (obj->hasSprite) {
		if (!obj->getSprite().expired()) {
			auto cSprite = obj->getSprite().lock();
			if (!cSprite->loaded) {
				cSprite->loadTextures(renderer);
				obj->updateSize();
			}
			cSprite->renderTimeBuffer += MSPerFrame;
			cSprite->render(renderer, obj->getPosition() + cSprite->getLocation() + positionMod);
			cSprite->tick();
		}
	}
	for (shared_ptr<GameObject> objC : obj->children) {
		renderGameObject(objC, obj->getPosition() + positionMod, MSPerFrame);
	}
}

shared_ptr<Renderer> World::getRenderer()
{
	return renderer;
}

void World::setDisplayFPS(bool enabled, shared_ptr<AssetLibrary> assetLibrary)
{
	World::displayFPS = enabled;
	World::assetLibrary = assetLibrary;
}

void World::setDisplayObjectCount(bool enabled, shared_ptr<AssetLibrary> assetLibrary)
{
	World::displayObjectCount = enabled;
	World::assetLibrary = assetLibrary;
}

void World::setDisplayHitboxes(bool enabled)
{
	World::displayHitboxes = enabled;
}

void World::refreshFPS()
{
	if (displayFPS) {
		if (!fpsIndicator) {
			string buffer;
			buffer += "FPS  " + to_string(cFPS) + " : " + (potentialFPS > 0 ? to_string(potentialFPS) : ">5000");
			fpsIndicator = make_shared<Text>(Text(buffer, debugFont, debugFontSize, { 255, 255, 255 }, Point(8, 6), assetLibrary));
			debugObjects.push_back(fpsIndicator);
		}
		else {
			string buffer;
			buffer += "FPS  " + to_string(cFPS) + " : " + (potentialFPS > 0 ? to_string(potentialFPS) : ">5000");
			fpsIndicator->setText(buffer, assetLibrary);
		}
	}
}

void World::refreshObjectCount()
{
	if (displayObjectCount) {
		if (!objectCountIndicator) {
			string buffer;
			int objC = (int)(currentScene->sprites.size()) + (int)(currentScene->getObjects().size());
			buffer += "OBJ  " + to_string(objC);
			objectCountIndicator = make_shared<Text>(Text(buffer, debugFont, debugFontSize, { 255, 255, 255 }, Point(8, 6*2 + debugFontSize), assetLibrary));
			debugObjects.push_back(objectCountIndicator);
		}
		else {
			string buffer;
			int objC = (int)(currentScene->sprites.size()) + (int)(currentScene->getObjects().size());
			buffer += "OBJ  " + to_string(objC);
			objectCountIndicator->setText(buffer, assetLibrary);
		}
	}
}

void World::addDebugObject(shared_ptr<GameObject> g)
{
	debugObjects.push_back(g);
}

string World::getDebugFont()
{
	return debugFont;
}

int World::getDebugFontSize()
{
	return debugFontSize;
}