#pragma once
#include <Renderer.h>
#include <Point.h>
#include <list>

class Sprite;
class GameObject;

class World {
public:
	World();
	void render(int frame, int fps);
	list<GameObject*> getObjects();
	void setRenderer(Renderer* renderer) {
		World::renderer = renderer;
	}
	Renderer* getRenderer() {
		return renderer;
	}
	Renderer* renderer;
	void destroy();
	std::list<Sprite*> sprites;
private:
	list<GameObject*> objects;
};