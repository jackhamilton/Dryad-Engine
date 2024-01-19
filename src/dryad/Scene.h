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
#include "Input.h"
#include "Polygon.h"
#include "Light.h"

namespace dryad
{

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

class Scene {
public:
	Scene();
	//gets all layers
	std::vector<std::vector<std::shared_ptr<GameObject>>> getObjects();
	//gets objects as a non-layered array
	std::vector<std::shared_ptr<GameObject>> getObjectsFlat();
	int getObjectsCount();
	std::vector<std::shared_ptr<Sprite>> sprites;
	void addObject(std::shared_ptr<GameObject> object, string layer);
	void addObject(std::shared_ptr<GameObject> object);
	void addSprite(std::shared_ptr<Sprite> sprite);
	void setCamera(std::shared_ptr<Camera> camera) {
		Scene::camera = camera;
	}

	std::shared_ptr<Renderer> renderer;
	string name;
	clock_t localTime;
	clock_t lastTickTime;
	double globalIllumination = 1.0;
private:
	//deactivates unique gameobjects of the same type as the sender (text field uniqueness, etc)
	void deactivateUniqueElements(GameObject* sender);
	//Sets the active text field pointer within the input class
	void activateTextField(TextField* object);
	void removeObject(GameObject* o);
	void handleObjectModificationQueue();
	bool raytrace(std::shared_ptr<Point> result, Point* origin, PolarVector* projection, std::vector<LineData>* testArray);
	bool onSameLine(Point* p1, std::shared_ptr<Point> p2, std::vector<LineData>* testArray);
	int nearestLine(Point p1, std::vector<LineData>* testArray);
	Point nearestPoint(Point p, std::vector<LineData>* testArray);
	std::vector<Polygon> generateSceneLightingMasks(Light l, Rectangle renderZone, Point cameraPMod);
	std::shared_ptr<Camera> camera;
	bool isCurrentScene;
	std::shared_ptr<Input> input;
	std::shared_ptr<std::shared_ptr<long double>> defaultFps;
	//set to actual time including delay
	std::shared_ptr<std::shared_ptr<long double>> lastFrameTimeMS;
	std::vector<GameObject*> postInputLoopObjectRemovalQueue;
	std::vector<pair<std::shared_ptr<GameObject>, string>> postInputLoopObjectAddQueue;
    map<string, std::vector<std::shared_ptr<GameObject>>> objects;
	friend class World;
};

}
