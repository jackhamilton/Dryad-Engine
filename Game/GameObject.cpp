#include <stdio.h>
#include "GameObject.h"
#include "Point.h"
#include "Line.h"
#include <time.h>

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
	if (size.width <= 0 && size.height <= 0) {
		if (!getSprite().expired()) {
			pair<int, int> sizes = getSprite().lock()->getDimensions();
			size = { sizes.first, sizes.second };
		}
	}
	return size;
}

void GameObject::setPosition(Point pos)
{
	GameObject::position = pos;
}

GameObject::GameObject(Point position) {
	hasSprite = false;
	hasMouseMoveEvent = false;
	hasMouseEnteredEvent = false;
	hasMouseExitedEvent = false;
	hasMouseClickEvent = false;
	hasMouseClickGraphicEvent = false;
	hasMouseRightClickEvent = false;
	hasMouseClickUpEvent = false;
	hasMouseClickUpGraphicEvent = false;
	hasMouseRightClickUpEvent = false;
	hitboxRendered = false;
	hitboxEnabled = false;
	if (cID == NULL) {
		id = 0;
		cID = 1;
	}
	else {
		id = cID;
		cID++;
	}
	GameObject::position = position;
}

GameObject::GameObject(Point position, shared_ptr<Sprite> sprite)
{
	hasSprite = true;
	hasMouseMoveEvent = false;
	hasMouseEnteredEvent = false;
	hasMouseExitedEvent = false;
	hasMouseClickEvent = false;
	hasMouseClickGraphicEvent = false;
	hasMouseRightClickEvent = false;
	hasMouseClickUpEvent = false;
	hasMouseClickUpGraphicEvent = false;
	hasMouseRightClickUpEvent = false;
	hitboxRendered = false;
	hitboxEnabled = false;
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

void GameObject::setSprite(shared_ptr<Sprite> sprite) {
	setSprite(sprite, false);
}

void GameObject::setSprite(shared_ptr<Sprite> sprite, bool deallocateOld) {
	hasSprite = true;
	if (deallocateOld) {
		GameObject::sprite.reset();
	}
	GameObject::sprite = sprite;
	if (sprite->loaded) {
		updateSize();
	}
}

weak_ptr<Sprite> GameObject::getSprite() {
	return weak_ptr<Sprite>(GameObject::sprite);
}

void GameObject::addChild(shared_ptr<GameObject> obj)
{
	children.push_back(obj);
}

void GameObject::queueEvent(shared_ptr<GameObjectEvent> event)
{
	eventQueue.push(event);
}

void GameObject::queueEvents(vector<shared_ptr<GameObjectEvent>> events)
{
	for (shared_ptr<GameObjectEvent> e : events) {
		eventQueue.push(e);
	}
}

//Gets all children and children of children recursive as a flat array
vector<shared_ptr<GameObject>> GameObject::getChildrenFlat()
{
	vector<shared_ptr<GameObject>> childrenFlat;
	//vector<shared_ptr<GameObject>> childChildren;
	for (shared_ptr<GameObject> o : children) {
		childrenFlat.push_back(o);
		for (shared_ptr<GameObject> ob : o->getChildrenFlat()) {
			childrenFlat.push_back(ob);
			//childChildren.push_back(ob);
		}
	}
	/*
	for (shared_ptr<GameObject> o : childChildren) {
		childrenFlat.push_back(o);
	}
	*/
	return childrenFlat;
}

//Only works if in current scene.
void GameObject::move(ModifiableProperty<Vector, double> vector)
{
	moveObject(vector);
}

//Only works if in current scene.
void GameObject::move(Vector vector)
{
	moveObject(ModifiableProperty<Vector, double>(vector));
}

//Speed can be affected by modifiers, including frame speed. Vector is px/sec. Set position for absolute movement.
void GameObject::moveObject(ModifiableProperty<Vector, double> vector)
{
	if (sceneActive) {
		std::vector<shared_ptr<Hitbox>> hitboxes;
		auto it = objects->begin();
		for (; it != objects->end(); it++) {
			for (int i = 0; i < it->second.size(); i++) {
				if (id != it->second.at(i)->id && it->second.at(i)->hitboxEnabled && it->second.at(i)->hitbox) {
					hitboxes.push_back(it->second.at(i)->hitbox);
				}
			}
		}
		double fpsSpeedFactor;
		if (!lastFrameTimeMS.expired() && *lastFrameTimeMS.lock()) {
			fpsSpeedFactor = (long double)(*lastFrameTimeMS.lock()) / 1000.0;
		}
		else if (!defaultFps.expired() && *defaultFps.lock()) {
			fpsSpeedFactor = 1.0 / (long double)(*defaultFps.lock());
		}
		else {
			fpsSpeedFactor = 1.0 / 60.0;
		}
		vector.addModifier(fpsSpeedFactor);
		pair<Vector, Collision> vecPair = hitbox->getMaximumClearDistanceForVectorFromGameObject(hitboxes, vector.getValue());
		if (vecPair.second.collided) {
			for (function<void(Point)> func : collisionEvents) {
				func(vecPair.second.p);
			}
		}
		Vector v = vecPair.first;
		Point adjustedPosition = position + v.getCartesian();
		setPosition(adjustedPosition);
	}
}

void GameObject::setPhysics(shared_ptr<Physics> p)
{
	physics = p;
}

shared_ptr<Physics> GameObject::getPhysics()
{
	return physics;
}

void GameObject::getMouseEvents(function<void()>* ret)
{
	ret[0] = mouseMoveEvent;
	ret[1] = mouseEnteredEvent;
	ret[2] = mouseExitedEvent;
	ret[3] = mouseClickGraphicEvent;
	ret[4] = mouseClickEvent;
	ret[5] = mouseRightClickEvent;
	ret[6] = mouseClickUpGraphicEvent;
	ret[7] = mouseClickUpEvent;
	ret[8] = mouseRightClickUpEvent;
}

void GameObject::removeFromParents()
{
	for (shared_ptr<GameObject> c : children) {
		c->removeFromParents();
	}
	for (function<void(GameObject*)> func : removeCalls) {
		func(this);
	}
}

bool GameObject::testInBounds(Point p)
{
	return (p.x > position.x and p.x < position.x + size.width and p.y > position.y and p.y < position.y + size.height);
}

void GameObject::enableHitbox()
{
	//called from world to set up a default hitbox after sprite size loads
	hitboxEnabled = true;
	Size objSize = getSize();
	if (objSize.width != 0 && objSize.height != 0) {
		Rectangle r;
		r.x = 0;
		r.y = 0;
		r.width = objSize.width;
		r.height = objSize.height;
		hitbox = make_shared<Hitbox>(Hitbox(r, &position));
	}
}

void GameObject::renderHitbox() {
	if (hitbox && !hitboxRendered) {
		vector<Point> corners = hitbox->getCorners();
		for (int i = 0; i < 4; i++) {
			auto l = shared_ptr<Line>(new Line({ 255, 0, 0 }, corners.at(i), corners.at(i == 3 ? 0 : i+1)));
			addChild(l);
		}
		hitboxRendered = true;
	}
}

Rectangle GameObject::getActiveRectangle()
{
	return {(int)position.x, (int)position.y, size.width, size.height};
}

void GameObject::addCollisionEvent(function<void(Point)> event)
{
	collisionEvents.push_back(event);
}

void GameObject::updateSize()
{
	pair<int, int> sizeOfSprite = sprite->getDimensions();
	size = { sizeOfSprite.first, sizeOfSprite.second };
}

void GameObject::handleEvents(clock_t ticksSinceLast)
{
	if (!eventQueue.empty()) {
		shared_ptr<GameObjectEvent> cEvent = eventQueue.front();
		while (!eventQueue.empty() && eventQueue.front()->completed) {
			eventQueue.pop();
		}
		if (eventQueue.empty()) {
			return;
		}
		switch (cEvent->type) {
		case EventType::DESTROY:
			removeFromParents();
			break;
		case EventType::DELAY:
		{
			shared_ptr<DelayEvent> eventAsProper = static_pointer_cast<DelayEvent>(cEvent);
			if (!eventAsProper->started) {
				eventAsProper->started = true;
			}
			if (eventAsProper->popTime <= eventAsProper->time) {
				eventQueue.pop();
				handleEvents(ticksSinceLast);
			}
			else {
				eventAsProper->time += (ticksSinceLast / (CLOCKS_PER_SEC / 1000));
			}
		}
			break;
		case EventType::WAIT_FOR_ANIMATION_COMPLETION:
			if (sprite->startedAnimation && (sprite->paused || sprite->isOnFinalFrame())) {
				eventQueue.pop();
				handleEvents(ticksSinceLast);
			}
			break;
		}
	}
}
