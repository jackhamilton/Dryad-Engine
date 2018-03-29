#include <stdio.h>
#include "GameObject.h"
#include "Point.h"

Point position;
Scene world;

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

void GameObject::setSprite(Sprite* sprite) {
	GameObject::sprite = sprite;
}

Sprite* GameObject::getSprite() {
	return GameObject::sprite;
}
