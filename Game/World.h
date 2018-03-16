#pragma once
#include <GameObject.h>
#include <Point.h>
#include <list>

class World {
public:
	World();
	list<GameObject> getObjects();
private:
	list<GameObject> objects;
protected:
	void destroy(GameObject object);
};