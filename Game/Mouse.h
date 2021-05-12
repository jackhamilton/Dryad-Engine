#pragma once
#include <functional>
#include <vector>
#include "Point.h"
#include "Rectangle.h"

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

using namespace std;
class Mouse
{
public:
	Point position;

	void addMouseMovementEvent(function<void(Point)> event);
	void addMouseClickEvent(function<void(Point)> event);
	void addMouseClickUpEvent(function<void(Point, Point)> event);
	/*
	static vector<function<void(Point)>> getMouseMovementEvents() {
		return mouseMovementEvents;
	}
	static vector<function<void(Point)>> getMouseClickEvents() {
		return mouseClickEvents;
	}
	static vector<function<void(Point, Point)>> getMouseClickUpEvents() {
		return mouseClickUpEvents;
	}
	static vector<function<void(Point)>> getSceneMouseMovementEvents() {
		return sceneMouseMovementEvents;
	}
	static vector<function<void(Point)>> getSceneMouseClickEvents() {
		return sceneMouseClickEvents;
	}
	static vector<function<void(Point, Point)>> getSceneMouseClickUpEvents() {
		return sceneMouseClickUpEvents;
	}*/
//private:
	vector <function<void(Point)>> mouseMovementEvents;
	vector <function<void(Point)>> mouseClickEvents;
	vector <function<void(Point)>> mouseRightClickEvents;
	//first point is down, second is up
	vector <function<void(Point, Point)>> mouseClickUpEvents;
	vector <function<void(Point, Point)>> mouseRightClickUpEvents;

	bool activeScene;
	vector<pair<Callback, Rectangle>>* sceneMouseMovementEvents;
	vector<pair<Callback, Rectangle>>* sceneMouseClickEvents;
	vector<pair<Callback, Rectangle>>* sceneMouseRightClickEvents;
	//first point is down, second is up
	vector<pair<Callback, Rectangle>>* sceneMouseClickUpEvents;
	vector<pair<Callback, Rectangle>>* sceneMouseRightClickUpEvents;
};

