#include <stdio.h>
#include "GameObject.h"
#include "Point.h"

static int cID = 0;

Point GameObject::getPosition()
{
	return GameObject::position;
}

void GameObject::setSize(Size size) {
	GameObject::size = size;
}

Size GameObject::getSize()
{
	return size;
}

void GameObject::setPosition(Point pos)
{
	GameObject::position = pos;
}

GameObject::GameObject(Point position) {
	if (cID == NULL) {
		id = 0;
		cID = 1;
	}
	else {
		id = cID;
		cID++;
	}
	GameObject::position = position;
	hasSprite = false;
}

GameObject::GameObject(Point position, Sprite* sprite)
{
	hasSprite = true;
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
	hasSprite = true;
	GameObject::sprite = sprite;
	if (!sprite->loaded) {
		addSpriteToSceneRenderQueue(sprite);
	}
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

void GameObject::getMouseEvents(function<void()>* ret)
{
	ret[0] = mouseMoveEvent;
	ret[1] = mouseEnteredEvent;
	ret[2] = mouseExitedEvent;
	ret[3] = mouseClickEvent;
	ret[4] = mouseClickGraphicEvent;
	ret[5] = mouseRightClickEvent;
	ret[6] = mouseClickUpEvent;
	ret[7] = mouseClickUpGraphicEvent;
	ret[8] = mouseRightClickUpEvent;
}

bool GameObject::testInBounds(Point p)
{
	return (p.x > position.x and p.x < position.x + size.width and p.y > position.y and p.y < position.y + size.height);
}

void GameObject::addSpriteToSceneRenderQueue(Sprite* s)
{
	renderQueue.push_back(s);
}
