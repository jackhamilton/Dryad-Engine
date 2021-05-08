#pragma once
#include "Point.h"
#include "Sprite.h"

class Physics;

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
	void setSprite(Sprite* sprite);
	Sprite* getSprite();
	void setPhysics(Physics* p);
	Physics* getPhysics();
	bool operator == (const GameObject& obj) const { return id == obj.id; }
	bool operator != (const GameObject& obj) const { return id != obj.id; }
private:
	int id;
	Point position;
	Sprite* sprite;
	Physics* physics;
};