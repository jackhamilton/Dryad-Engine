#pragma once
#include "Point.h"
#include "Sprite.h"
#include <Size.h>
#include <functional>

class Physics;

using namespace std;
class GameObject {
public:
	bool hasSprite;
	Point getPosition();
	void setPosition(Point pos);
	GameObject() : GameObject(0, 0) {};
	GameObject(double x, double y) : GameObject(Point(x, y)) {};
	GameObject(Point location);
	GameObject(double x, double y, Sprite* sprite) : GameObject(Point(), sprite) {};
	GameObject(Point position, Sprite* sprite);
	void setSize(Size size);
	Size getSize();
	void setSprite(Sprite* sprite);
	Sprite* getSprite();
	void setPhysics(Physics* p);
	Physics* getPhysics();
	bool operator == (const GameObject& obj) const { return id == obj.id; }
	bool operator != (const GameObject& obj) const { return id != obj.id; }
	void getMouseEvents(function<void()>* ret);
	bool hasMouseMoveEvent;
	bool hasMouseClickEvent;
	bool hasMouseRightClickEvent;
	bool hasMouseClickUpEvent;
	bool hasMouseRightClickUpEvent;
	//check if a point is inside the object. Size must be set.
	bool testInBounds(Point p);
protected:
	function<void()> mouseMoveEvent;
	function<void()> mouseClickEvent;
	function<void()> mouseRightClickEvent;
	function<void()> mouseClickUpEvent;
	function<void()> mouseRightClickUpEvent;
private:
	int id;
	Point position;
	Size size;
	Sprite* sprite;
	Physics* physics;
};