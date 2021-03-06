#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include <functional>
#include "Point.h"
#include "Sprite.h"
#include "Size.h"
#include "Vector.h"
#include "Hitbox.h"
#include "GameObjectEvent.h"
#include "Polygon.h"

class Physics;
class Hitbox;

using namespace std;
class GameObject {
public:
	bool hasSprite = false;
	Point getPosition();
	void setPosition(Point pos);
	GameObject() : GameObject(0, 0) {};
	GameObject(double x, double y) : GameObject(Point(x, y)) {};
	GameObject(Point location);
	GameObject(double x, double y, shared_ptr<Sprite> sprite) : GameObject(Point(), sprite) {};
	GameObject(Point position, shared_ptr<Sprite> sprite);

	vector<weak_ptr<Sprite>> renderQueue;

	void setSize(Size size);
	Size getSize();
	void setSprite(shared_ptr<Sprite> sprite);
	void setSprite(shared_ptr<Sprite> sprite, bool deallocateOld);
	weak_ptr<Sprite> getSprite();
	void addChild(shared_ptr<GameObject> obj);
	vector<shared_ptr<GameObject>> getChildrenFlat();
	void queueEvent(shared_ptr<GameObjectEvent> event);
	void queueEvents(vector<shared_ptr<GameObjectEvent>> events);
	void move(ModifiableProperty<Vector, double> vector);
	void move(Vector vector);
	void setPhysics(shared_ptr<Physics> p);
	shared_ptr<Physics> getPhysics();
	void getMouseEvents(function<void()>* ret);
	void removeFromParents();
	//check if a point is inside the object. Size must be set.
	bool testInBounds(Point p);
	void enableHitbox();
	void renderHitbox();
	Rectangle getActiveRectangle();
	//TODO: void enableHitbox(Hitbox h);
	//camera tracking affects object?
	void setIsUIElement(bool isUIElement) {
		GameObject::isUIElement = isUIElement;
	}

	//GameObjects should inherit the highest lighting from any mask touching them. if EdgeIlluminated is true,
	//each side should be dynamically extrapolated from surrounding light and the values should fade.
	bool edgeIlluminated = false;
	bool blocksLighting = true;

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
	function<void()> onEnteringScene;
	function<void()> onExitingScene;
	//currently, children render on top
	vector<shared_ptr<GameObject>> children;
	//TODO: these do not work on moving gameobjects due to passing a rectangle as callback. Change to GameObject* or Rectangle*.
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
	queue<shared_ptr<GameObjectEvent>> eventQueue;
	vector<function<void(GameObject*)>> removeCalls;
	int id;
	shared_ptr<Hitbox> hitbox;
	bool hitboxEnabled;
	bool hitboxRendered;
	Point position;
	Size size;
	shared_ptr<Sprite> sprite;
	shared_ptr<Physics> physics;
	void addCollisionEvent(function<void(Point)> event);
	//camera tracking
	bool isUIElement = false;
	bool isTextField = false;
	bool isVerticalStackFrame = false;
	bool isHorizontalStackFrame = false;
private:
	bool sceneActive = false;
	map<string, vector<shared_ptr<GameObject>>>* objects;
	weak_ptr<long double> defaultFps;
	//set to actual time including delay
	weak_ptr<long double> lastFrameTimeMS;
	Polygon getLightingMask(Point cameraPMod);
	void updateSize();
	void moveObject(ModifiableProperty<Vector, double> vector);
	void handleEvents(clock_t ticksSinceLast);
	friend class World;
	friend class Scene;
	friend class Input;
	friend class Hitbox;
	friend class HorizontalStackFrame;
	friend class VerticalStackFrame;
	friend class UIBlock;
};