#pragma once
#include <SDL.h>
#include "Sprite.h"
#include <vector>

class Renderer;

class Spritesheet : public Sprite {
public:
	Spritesheet();
    Spritesheet(std::vector<char*> filename, std::vector<int> framecounts, std::vector<bool> looping, std::vector<int> heights, int width);
	void getCurrentFrame(int* dim);
	void setCurrentAnimation(int currentAnimation) {
		if (currentAnimation != Spritesheet::currentAnimation) {
			Spritesheet::currentAnimation = currentAnimation;
			currentFrame = 0;
			paused = false;
		}
	}
	void destroy() {
		Sprite::destroy();
	}
	void nextImage();
	void render(Renderer* renderer, Point locationMod);
	int getWidth();
	int getHeight();
	std::pair<int, int> getDimensions();
private:
    std::vector<int> framecounts;
    std::vector<int> heights;
	std::vector<bool> looping;
	int width;
	int currentFrame;
	int currentAnimation;
};