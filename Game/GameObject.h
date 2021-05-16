#pragma once
#include <vector>
#include <queue>
#include <functional>
#include "Point.h"
#include "Sprite.h"
#include "Size.h"
#include "Vector.h"
#include "Hitbox.h"
#include "GameObjectEvent.h"

class Physics;
class Hitbox;

using namespace std;
class GameObject {
public:
	bool hasSprite;
	Point getPosition();
	void setPosition(Point pos);
	GameObject() : GameObject(0, 0) {};
	GameObject(double x, double y) : GameObject(Point(x, y)) {};
	GameObject(Point location);
	GameObject(double x, double y, Sprite* sprite) : GameObject(Point(), sprite) {};
	GameObject(Point position, Sprite* sprite);
	vector<Sprite*> renderQueue;

	void setSize(Size size);
	Size getSize();
	void setSprite(Sprite* sprite);
	Sprite* getSprite();
	void addChild(GameObject* obj);
	void queueEvent(GameObjectEvent* event);
	void queueEvents(vector<GameObjectEvent*> events);
	void move(ModifiableProperty<Vector, double> vector);
	void move(Vector vector);
	void setPhysics(Physics* p);
	Physics* getPhysics();
	void getMouseEvents(function<void()>* ret);
	void removeFromParents();
	//check if a point is inside the object. Size must be set.
	bool testInBounds(Point p);
	void enableHitbox();
	void renderHitbox();
	//TODO: void enableHitbox(Hitbox h);

	bool operator == (const GameObject& obj) const { return id == obj.id; }
	bool operator != (const GameObject& obj) const { return id != obj.id; }
	//TODO: REWORK THIS WHOLE THING'S VISIBLITY USING FRIEND CLASSES (also in input, mouse, scene classes)
	bool hasMouseMoveEvent;
	bool hasMouseEnteredEvent;
	bool hasMouseExitedEvent;
	bool hasMouseClickEvent;
	bool hasMouseClickGraphicEvent;
	bool hasMouseRightClickEvent;
	bool hasMouseClickUpEvent;
	bool hasMouseClickUpGraphicEvent;
	bool hasMouseRightClickUpEvent;
protected:
	//currently, children render on top
	vector<GameObject*> children;
	function<void()> mouseMoveEvent;
	function<void()> mouseEnteredEvent;
	function<void()> mouseExitedEvent;
	function<void()> mouseClickEvent;
	function<void()> mouseClickGraphicEvent;
	function<void()> mouseRightClickEvent;
	function<void()> mouseClickUpEvent;
	function<void()> mouseClickUpGraphicEvent;
	function<void()> mouseRightClickUpEvent;
	vector<function<void(Point)>> collisionEvents;
	queue<GameObjectEvent*> eventQueue;
	vector<function<void(GameObject*)>> removeCalls;
	int id;
	Hitbox* hitbox;
	bool hitboxEnabled;
	bool hitboxRendered;
	Point position;
	Size size;
	Sprite* sprite;
	Physics* physics;
	function<void(GameObject*, ModifiableProperty<Vector, double>)>* movementCallback;
	void addSpriteToSceneRenderQueue(Sprite* s);
	void addCollisionEvent(function<void(Point)> event);
private:
	void updateSize();
	void handleEvents();
	friend class World;
	friend class Scene;
	friend class Hitbox;
};