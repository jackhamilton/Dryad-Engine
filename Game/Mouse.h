#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "Point.h"
#include "Rectangle.h"
#include "GameObject.h"

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

using namespace std;
class Mouse
{
public:
	Mouse();
	Point position;

	void addMouseMovementEvent(function<void(Point)> event);
	void addMouseClickEvent(function<void(Point)> event);
	void addMouseClickUpEvent(function<void(Point, Point)> event);

	vector <function<void(Point)>> mouseMovementEvents;
	vector <function<void(Point)>> mouseClickEvents;
	vector <function<void(Point)>> mouseRightClickEvents;
	vector <function<void(Point, Point)>> mouseClickUpEvents;
	vector <function<void(Point, Point)>> mouseRightClickUpEvents;

	map<string, vector<shared_ptr<GameObject>>>* sceneObjectSet;

	bool activeScene;
};

