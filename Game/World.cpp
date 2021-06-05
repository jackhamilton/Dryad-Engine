#include "World.h"
#include <stdio.h>
#include "GameObjectEvent.h"
#include <vector>
#include "Polygon.h"
#include "Light.h"

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
	//Exit current scene
	if (currentScene) {
		for (vector<shared_ptr<GameObject>> oVec : currentScene->getObjects()) {
			for (shared_ptr<GameObject> o : oVec) {
				o->sceneActive = false;
				if (o->onExitingScene) {
					o->onExitingScene();
				}
			}
		}
		for (vector<shared_ptr<GameObject>> oVec : currentScene->getObjects()) {
			for (shared_ptr<GameObject> o : oVec) {
				o->sceneActive = true;
				if (o->hasMouseExitedEvent) {
					o->mouseExitedEvent();
				}
				if (o->mouseExitedEvent) {
					o->mouseExitedEvent();
				}
			}
		}
		currentScene->isCurrentScene = false;
		currentScene->input = nullptr;
	}
	//Initialize new scene
	currentScene = scenes[name];
	mouse->activeScene = true;
	mouse->sceneObjectSet = &(currentScene->objects);
	currentScene->isCurrentScene = true;
	currentScene->input = input;
	camera = currentScene->camera;
	for (vector<shared_ptr<GameObject>> oVec : currentScene->getObjects()) {
		for (shared_ptr<GameObject> o : oVec) {
			o->sceneActive = true;
			if (o->onEnteringScene) {
				o->onEnteringScene();
			}
			o->objects = &(currentScene->objects);
			o->defaultFps = weak_ptr<long double>(defaultFps);
			o->lastFrameTimeMS = weak_ptr<long double>(defaultFps);
		}
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
		currentScene->handleObjectModificationQueue();
		Point cameraPositionModifier = Point(0, 0);
		if (camera) {
			cameraPositionModifier = camera->getPositionModifier();
		}
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
			cSprite->render(renderer, location + cameraPositionModifier);
			cSprite->tick();
		}
		//Do the same thing but for GameObjects
		for (vector<shared_ptr<GameObject>> oVec : currentScene->getObjects()) {
			for (shared_ptr<GameObject> obj : oVec) {
				if (obj->isUIElement) {
					renderGameObject(obj, Point(0, 0), MSPerFrame);
				}
				else {
					renderGameObject(obj, cameraPositionModifier, MSPerFrame);
				}
				obj->handleEvents(currentScene->localTime - currentScene->lastTickTime);
			}
		}
		//Then draw debug lighting rectangles
		if (camera) {
			Light l;
			l.p = input->getMouse()->position;
			Rectangle renderBox;
			renderBox.x = 0;
			renderBox.y = 0;
			pair<int, int> dim = Window::calculateResolution(camera->resolution);
			renderBox.width = dim.first;
			renderBox.height = dim.second;
			vector<Polygon> masks = currentScene->generateSceneLightingMasks(l, renderBox, cameraPositionModifier);
			for (Polygon p : masks) {
				if (p.shape.size() == 3) {
					//renderer->drawFilledTriangle(p, { 255, 0, 0 });
				}
				else {
					printf("Error generating lighting polygons");
				}
			}
			vector<Light> sceneLights;
			vector<vector<Polygon>> correspondingMasks;
			sceneLights.push_back(l);
			correspondingMasks.push_back(masks);
			renderer->computeLighting(currentScene->globalIllumination, make_pair(sceneLights, correspondingMasks));
		}
		else {
			Light l;
			l.p = input->getMouse()->position;
			Rectangle renderBox;
			renderBox.x = 0;
			renderBox.y = 0;
			pair<int, int> dim = Window::calculateResolution(renderer->res);
			renderBox.width = dim.first;
			renderBox.height = dim.second;
			vector<Polygon> masks = currentScene->generateSceneLightingMasks(l, renderBox, cameraPositionModifier);
			for (Polygon p : masks) {
				if (p.shape.size() == 3) {
					//renderer->drawFilledTriangle(p, { 255, 0, 0 });
				}
				else {
					printf("Error generating lighting polygons");
				}
			}
			vector<Light> sceneLights;
			vector<vector<Polygon>> correspondingMasks;
			sceneLights.push_back(l);
			correspondingMasks.push_back(masks);
			renderer->computeLighting(currentScene->globalIllumination, make_pair(sceneLights, correspondingMasks));
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
			int objC = (int)(currentScene->sprites.size()) + (int)(currentScene->getObjectsCount());
			buffer += "OBJ  " + to_string(objC);
			objectCountIndicator = make_shared<Text>(Text(buffer, debugFont, debugFontSize, { 255, 255, 255 }, Point(8, 6*2 + debugFontSize), assetLibrary));
			debugObjects.push_back(objectCountIndicator);
		}
		else {
			string buffer;
			int objC = (int)(currentScene->sprites.size()) + (int)(currentScene->getObjectsCount());
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