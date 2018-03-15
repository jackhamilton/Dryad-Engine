#include <stdio.h>
#include <GameObject.h>
#include <Point.h>
#include <World.h>

Point position;
World world;

static int cID = 0;

GameObject::GameObject(Point position)
{
	if (cID == NULL) {
		id = 0;
		cID = 1;
	}
	else {
		id = cID;
		cID++;
	}
	::position = position;
}
