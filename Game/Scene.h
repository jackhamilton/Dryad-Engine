#pragma once
#include "Renderer.h"
#include "Point.h"
#include <list>
#include <GameObject.h>
#include <vector>
#include <Sprite.h>

class Scene {
public:
	Scene();
	void render(int frame, int fps);
    std::vector<GameObject*> getObjects();
	void setRenderer(Renderer* renderer) {
		Scene::renderer = renderer;
	}
	Renderer* getRenderer() {
		return renderer;
	}
	Renderer* renderer;
	void destroy();
	std::vector<Sprite*> sprites;
	void addObject(GameObject* object);
	void addSprite(Sprite* sprite);
private:
    std::vector<GameObject*> objects;
};
