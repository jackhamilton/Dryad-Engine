#include "World.h"
#include <Point.h>
#include <list>

World::World()
{

}

list<GameObject> World::getObjects()
{
	return objects;
}

void World::destroy(GameObject object)
{
	objects.remove(object);
	delete &object;
}
