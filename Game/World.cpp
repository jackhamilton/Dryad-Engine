#include "World.h"
#include <stdio.h>

World::World(Renderer* renderer, Input* input)
{
	World::renderer = renderer;
	World::input = input;
	World::mouse = input->getMouse();
}

void World::addScene(Scene* s, const char* name)
{
	s->renderer = renderer;
	s->name = name;
	scenes[name] = s;
	if (!currentScene) {
		setScene(name);
	}
}

void World::setScene(const char* name)
{
	if (mouse->sceneMouseExitedEvents) {
		vector<pair<function<void()>, Rectangle>> vec = *mouse->sceneMouseExitedEvents;
		for (vector<pair<function<void()>, Rectangle>>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			pair<function<void()>, Rectangle> pair = *it;
			function<void()> func = pair.first;
			func();
		}
	}
	currentScene = scenes[name];
	mouse->sceneMouseClickEvents = &currentScene->sceneMouseClickEvents;
	mouse->sceneMouseEnteredEvents = &currentScene->sceneMouseEnteredEvents;
	mouse->sceneMouseExitedEvents = &currentScene->sceneMouseExitedEvents;
	mouse->sceneMouseClickUpEvents = &currentScene->sceneMouseClickUpEvents;
	mouse->sceneMouseMovementEvents = &currentScene->sceneMouseMovementEvents;
	mouse->sceneMouseRightClickEvents = &currentScene->sceneMouseRightClickEvents;
	mouse->sceneMouseRightClickUpEvents = &currentScene->sceneMouseRightClickUpEvents;
	mouse->activeScene = true;
}

string World::getCurrentSceneName()
{
	return currentScene->name;
}

Scene* World::getCurrentScene()
{
	return currentScene;
}

void World::setRenderer(Renderer* renderer)
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
		for (Sprite* cSprite : currentScene->sprites) {
			if (!cSprite->loaded) {
				cSprite->loadTextures(renderer);
			}
			cSprite->renderTimeBuffer += MSPerFrame;
			Point location = cSprite->getLocation();
			cSprite->render(renderer, location);
			cSprite->tick();
		}
		//Do the same thing but for GameObjects
		for (GameObject* obj : currentScene->getObjects()) {
			renderGameObject(obj, Point(0, 0), MSPerFrame);
		}
		//And finally for the debug layer
		for (GameObject* obj : debugObjects) {
			renderGameObject(obj, Point(0, 0), MSPerFrame);
		}
		renderer->renderPresent();
	}
}

void World::renderGameObject(GameObject* obj, Point positionMod, double MSPerFrame) {
	if (obj->hitboxEnabled && !obj->hitbox) {
		obj->enableHitbox();
	}
	if (displayHitboxes && obj->hitboxEnabled && !obj->hitboxRendered) {
		obj->renderHitbox();
	}
	if (obj->hasSprite) {
		Sprite* cSprite = obj->getSprite();
		if (!cSprite->loaded) {
			cSprite->loadTextures(renderer);
			obj->updateSize();
		}
		cSprite->renderTimeBuffer += MSPerFrame;
		cSprite->render(renderer, Point(obj->getPosition().x + positionMod.x, obj->getPosition().y + positionMod.y));
		cSprite->tick();
	}
	for (GameObject* objC : obj->children) {
		renderGameObject(objC, obj->getPosition(), MSPerFrame);
	}
}

Renderer* World::getRenderer()
{
	return renderer;
}

void World::destroy()
{
	for (map<const char*, Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
		it->second->destroy();
	}
}

void World::setDisplayFPS(bool enabled)
{
	World::displayFPS = enabled;
}

void World::setDisplayObjectCount(bool enabled)
{
	World::displayObjectCount = enabled;
}

void World::setDisplayHitboxes(bool enabled)
{
	World::displayHitboxes = enabled;
}

void World::refreshFPS()
{
	if (displayFPS) {
		if (!fpsIndicator) {
			char buffer[40];
			sprintf(buffer, "FPS  %d : %d", cFPS, potentialFPS);
			fpsIndicator = new Text(buffer, debugFont, debugFontSize, { 255, 255, 255 }, Point(8, 6));
			debugObjects.push_back(fpsIndicator);
		}
		else {
			char buffer[40];
			sprintf(buffer, "FPS  %d : %d", cFPS, potentialFPS);
			fpsIndicator->setText(buffer);
		}
	}
}

void World::refreshObjectCount()
{
	if (displayObjectCount) {
		if (!objectCountIndicator) {
			char buffer[40];
			int objC = currentScene->sprites.size() + currentScene->getObjects().size();
			sprintf(buffer, "OBJ  %d", objC);
			objectCountIndicator = new Text(buffer, debugFont, debugFontSize, { 255, 255, 255 }, Point(8, 6*2 + debugFontSize));
			debugObjects.push_back(objectCountIndicator);
		}
		else {
			char buffer[40];
			int objC = currentScene->sprites.size() + currentScene->getObjects().size();
			sprintf(buffer, "OBJ  %d", objC);
			objectCountIndicator->setText(buffer);
		}
	}
}

void World::addDebugObject(GameObject* g)
{
	debugObjects.push_back(g);
}

const char* World::getDebugFont()
{
	return debugFont;
}

int World::getDebugFontSize()
{
	return debugFontSize;
}
