#pragma once
#include <Point.h>

class World;

class GameObject {
public:
	Point getPosition() { return position; };
	GameObject(int x, int y) : GameObject(*(new Point(x, y))) {}; //This pointer might create a memory leak.
	GameObject(Point position);
	bool operator == (const GameObject& obj) const { return id == obj.id; }
	bool operator != (const GameObject& obj) const { return id != obj.id; }
private:
	int id;
	Point position;
};