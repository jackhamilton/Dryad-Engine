#pragma once
#include <Point.h>

class World;

class GameObject {
public:
	Point* getPosition() { return position; };
	GameObject(int x, int y) : GameObject(new Point(x, y)) {};
	GameObject(Point* position);
	void destroy();
	bool operator == (const GameObject& obj) const { return id == obj.id; }
	bool operator != (const GameObject& obj) const { return id != obj.id; }
private:
	int id;
	Point* position;
	World* world;
protected: 
	void setWorld(World* world);
};