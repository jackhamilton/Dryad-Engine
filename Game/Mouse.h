#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "Point.h"
#include "Rectangle.h"

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

	bool activeScene;
	//TODO: add a quadruplet with two callbacks: entered() + mouseLeft(). On scene transition call all mouseLeft().
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseMovementEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseEnteredEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseExitedEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseClickEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseRightClickEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseClickUpEvents;
	shared_ptr<vector<pair<Callback, Rectangle>>> sceneMouseRightClickUpEvents;
};

