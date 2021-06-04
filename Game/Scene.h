#pragma once
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include "Renderer.h"
#include "Point.h"
#include "Camera.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Rectangle.h"
#include "Vector.h"
#include "Input.h"
#include "Polygon.h"
#include "Light.h"


//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

using namespace std;
class Scene {
public:
	Scene();
	//gets all layers
	vector<vector<shared_ptr<GameObject>>> getObjects();
	//gets objects as a non-layered array
	std::vector<shared_ptr<GameObject>> getObjectsFlat();
	int getObjectsCount();
	vector<shared_ptr<Sprite>> sprites;
	void addObject(shared_ptr<GameObject> object, string layer);
	void addObject(shared_ptr<GameObject> object);
	void addSprite(shared_ptr<Sprite> sprite);
	void setCamera(shared_ptr<Camera> camera) {
		Scene::camera = camera;
	}

	shared_ptr<Renderer> renderer;
	string name;
	clock_t localTime;
	clock_t lastTickTime;
private:
	//deactivates unique gameobjects of the same type as the sender (text field uniqueness, etc)
	void deactivateUniqueElements(GameObject* sender);
	//Sets the active text field pointer within the input class
	void activateTextField(TextField* object);
	void removeObject(GameObject* o);
	void handleObjectModificationQueue();
	bool raytrace(shared_ptr<Point> result, Point* origin, PolarVector* projection, vector<LineData>* testArray);
	bool onSameLine(Point* p1, shared_ptr<Point> p2, vector<LineData>* testArray);
	int nearestLine(Point p1, vector<LineData>* testArray);
	Point nearestPoint(Point p, vector<LineData>* testArray);
	vector<Polygon> generateSceneLightingMasks(Light l, Rectangle renderZone, Point cameraPMod);
	shared_ptr<Camera> camera;
	bool isCurrentScene;
	shared_ptr<Input> input;
	shared_ptr<shared_ptr<long double>> defaultFps;
	//set to actual time including delay
	shared_ptr<shared_ptr<long double>> lastFrameTimeMS;
	vector<GameObject*> postInputLoopObjectRemovalQueue;
	vector<pair<shared_ptr<GameObject>, string>> postInputLoopObjectAddQueue;
    map<string, vector<shared_ptr<GameObject>>> objects;
	friend class World;
};
