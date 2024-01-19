#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "Point.h"
#include "Rectangle.h"
#include "GameObject.h"

namespace dryad
{

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

class Mouse
{
public:
	Mouse();
	Point position;

	void addMouseMovementEvent(function<void(Point)> event);
	void addMouseClickEvent(function<void(Point)> event);
	void addMouseClickUpEvent(function<void(Point, Point)> event);

	std::vector <function<void(Point)>> mouseMovementEvents;
	std::vector <function<void(Point)>> mouseClickEvents;
	std::vector <function<void(Point)>> mouseRightClickEvents;
	std::vector <function<void(Point, Point)>> mouseClickUpEvents;
	std::vector <function<void(Point, Point)>> mouseRightClickUpEvents;

	std::map<string, std::vector<shared_ptr<GameObject>>>* sceneObjectSet;

	bool activeScene;
};

}
