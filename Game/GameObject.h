#pragma once
#include <vector>
#include <functional>
#include "Point.h"
#include "Sprite.h"
#include "Size.h"
#include "Vector.h"

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
	void setSize(Size size);
	Size getSize();
	void setSprite(Sprite* sprite);
	Sprite* getSprite();
	vector<Sprite*> renderQueue;

	void move(ModifiableProperty<Vector, double> vector);

	void setPhysics(Physics* p);
	Physics* getPhysics();
	bool operator == (const GameObject& obj) const { return id == obj.id; }
	bool operator != (const GameObject& obj) const { return id != obj.id; }
	void getMouseEvents(function<void()>* ret);
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
	//check if a point is inside the object. Size must be set.
	bool testInBounds(Point p);
protected:
	function<void()> mouseMoveEvent;
	function<void()> mouseEnteredEvent;
	function<void()> mouseExitedEvent;
	function<void()> mouseClickEvent;
	function<void()> mouseClickGraphicEvent;
	function<void()> mouseRightClickEvent;
	function<void()> mouseClickUpEvent;
	function<void()> mouseClickUpGraphicEvent;
	function<void()> mouseRightClickUpEvent;
	int id;
	Hitbox* hitbox;
	Point position;
	Size size;
	Sprite* sprite;
	Physics* physics;
	function<void(GameObject*, ModifiableProperty<Vector, double>)>* movementCallback;
	void addSpriteToSceneRenderQueue(Sprite* s);
private:
	friend class Scene;
	friend class Hitbox;
};