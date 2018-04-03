#pragma once
#include "Point.h"

class Sprite;

class GameObject {
public:
	Point getLocation();
	void setLocation(Point pos);
	GameObject() : GameObject(0, 0) {};
	GameObject(double x, double y) : GameObject(Point(), nullptr) {}; //This pointer might create a memory leak.
	GameObject(double x, double y, Sprite* sprite) : GameObject(Point(), sprite) {};
	GameObject(Point position, Sprite* sprite);
	void setSprite(Sprite* sprite);
	Sprite* getSprite();
	bool operator == (const GameObject& obj) const { return id == obj.id; }
	bool operator != (const GameObject& obj) const { return id != obj.id; }
private:
	int id;
	Point position;
	Sprite* sprite;
};
