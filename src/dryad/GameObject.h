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

namespace dryad
{

class Physics;
class Hitbox;

class GameObject {
public:
	bool hasSprite = false;
	Point getPosition();
	void setPosition(Point pos);
	GameObject() : GameObject(0, 0) {};
	GameObject(double x, double y) : GameObject(Point(x, y)) {};
	GameObject(Point location);
	GameObject(double x, double y, std::shared_ptr<Sprite> sprite) : GameObject(Point(), sprite) {};
	GameObject(Point position, std::shared_ptr<Sprite> sprite);

    std::vector<std::weak_ptr<Sprite>> renderQueue;

	void setSize(Size size);
	Size getSize();
	void setSprite(std::shared_ptr<Sprite> sprite);
	void setSprite(std::shared_ptr<Sprite> sprite, bool deallocateOld);
	std::weak_ptr<Sprite> getSprite();
	void addChild(std::shared_ptr<GameObject> obj);
    std::vector<std::shared_ptr<GameObject>> getChildrenFlat();
	void queueEvent(std::shared_ptr<GameObjectEvent> event);
	void queueEvents(std::vector<std::shared_ptr<GameObjectEvent>> events);
	void move(ModifiableProperty<Vector, double> vector);
	void move(Vector vector);
	void setPhysics(std::shared_ptr<Physics> p);
	std::shared_ptr<Physics> getPhysics();
	void getMouseEvents(std::function<void()>* ret);
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
	std::function<void()> onEnteringScene;
	std::function<void()> onExitingScene;
	//currently, children render on top
	std::vector<std::shared_ptr<GameObject>> children;
	//TODO: these do not work on moving gameobjects due to passing a rectangle as callback. Change to GameObject* or Rectangle*.
	std::function<void()> mouseMoveEvent;
	std::function<void()> mouseEnteredEvent;
	std::function<void()> mouseExitedEvent;
	std::function<void()> mouseClickEvent;
	std::function<void()> mouseClickGraphicEvent;
	std::function<void()> mouseRightClickEvent;
	std::function<void()> mouseClickUpEvent;
	std::function<void()> mouseClickUpGraphicEvent;
	std::function<void()> mouseRightClickUpEvent;
	std::vector<std::function<void(Point)>> collisionEvents;
    std::queue<std::shared_ptr<GameObjectEvent>> eventQueue;
	std::vector<std::function<void(GameObject*)>> removeCalls;
	int id;
	std::shared_ptr<Hitbox> hitbox;
	bool hitboxEnabled;
	bool hitboxRendered;
	Point position;
	Size size;
	std::shared_ptr<Sprite> sprite;
	std::shared_ptr<Physics> physics;
	void addCollisionEvent(std::function<void(Point)> event);
	//camera tracking
	bool isUIElement = false;
	bool isTextField = false;
	bool isVerticalStackFrame = false;
	bool isHorizontalStackFrame = false;
private:
	bool sceneActive = false;
    std::map<std::string, std::vector<std::shared_ptr<GameObject>>>* objects;
	std::weak_ptr<long double> defaultFps;
	//set to actual time including delay
	std::weak_ptr<long double> lastFrameTimeMS;
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

}