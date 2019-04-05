#include <stdio.h>
#include "GameObject.h"
#include "Point.h"

static int cID = 0;

Point GameObject::getPosition()
{
	return GameObject::position;
}

void GameObject::setPosition(Point pos)
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

void GameObject::setPhysics(Physics * p)
{
	physics = p;
}

Physics* GameObject::getPhysics()
{
	return physics;
}
