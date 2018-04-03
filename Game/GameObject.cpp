#include <stdio.h>
#include "GameObject.h"
#include "Point.h"

static int cID = 0;

Point GameObject::getLocation()
{
	return GameObject::position;
}

void GameObject::setLocation(Point pos)
{
	GameObject::position = pos;
}

GameObject::GameObject(Point position, Sprite* sprite)
{
	if (cID == NULL) {
		id = 0;
		cID = 1;
	}
	else {
		id = cID;
		cID++;
	}
	GameObject::position = position;
	setSprite(sprite);
}

void GameObject::setSprite(Sprite* sprite) {
	GameObject::sprite = sprite;
}

Sprite* GameObject::getSprite() {
	return GameObject::sprite;
}
