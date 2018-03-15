#include "World.h"
#include <Point.h>
#include <list>

World::World()
{
	objects = new list<GameObject>;
}

list<GameObject>* World::getObjects()
{
	return objects;
}

void World::destroy(GameObject object)
{
	objects->remove(object);
	delete &object;
}

void World::destroy()
{
	delete objects;
}
