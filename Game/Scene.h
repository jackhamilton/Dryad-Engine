#pragma once
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include "Renderer.h"
#include "Point.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Rectangle.h"
#include "Vector.h"

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

using namespace std;
class Scene {
public:
	Scene();
	vector<vector<shared_ptr<GameObject>>> getObjects();
	vector<shared_ptr<Sprite>> sprites;
	void addObject(shared_ptr<GameObject> object, string scene);
	void addObject(shared_ptr<GameObject> object);
	void addSprite(shared_ptr<Sprite> sprite);
	int getObjectsCount();

	//Engine objects - do not modify without very good reason
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseMovementEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseEnteredEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseExitedEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseClickEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseRightClickEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseClickUpEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseRightClickUpEvents;
	shared_ptr<Renderer> renderer;
	string name;
	clock_t localTime;
	clock_t lastTickTime;
private:
	bool isCurrentScene;
	shared_ptr<shared_ptr<long double>> defaultFps;
	//set to actual time including delay
	shared_ptr<shared_ptr<long double>> lastFrameTimeMS;
    map<string, vector<shared_ptr<GameObject>>> objects;
	void removeObject(GameObject* o);
	friend class World;
};
