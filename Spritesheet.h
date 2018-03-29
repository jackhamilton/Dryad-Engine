#pragma once
#include <SDL2/SDL.h>
#include "Sprite.h"
#include <vector>

class Spritesheet : public Sprite {
public:
    Spritesheet(std::vector<char*> filename, std::vector<int> framecounts, std::vector<int> heights, int width, Scene* window);
	void getCurrentFrame(int* dim);
	void setCurrentAnimation(int currentAnimation) {
		if (currentAnimation != Spritesheet::currentAnimation) {
			Spritesheet::currentAnimation = currentAnimation;
			currentFrame = 0;
		}
	}
	void destroy() {
		Sprite::destroy();
	}
	void nextFrame();
	int getWidth();
	int getHeight();
private:
    std::vector<int> framecounts;
    std::vector<int> heights;
	int width;
	int currentFrame;
	int currentAnimation;
};
