#pragma once
#include <SDL.h>
#include <Sprite.h>

class Spritesheet : public Sprite {
public:
	Spritesheet(const char* filename[], int* framecounts, int* heights, int width, Scene* window);
	void getCurrentFrame(int* dim);
	void setCurrentAnimation(int currentAnimation) {
		if (currentAnimation != Spritesheet::currentAnimation) {
			Spritesheet::currentAnimation = currentAnimation;
			currentFrame = 0;
		}
	}
	void destroy() {
		Sprite::destroy();
		delete heights;
	}
	void nextFrame();
	int getWidth();
	int getHeight();
private:
	int* framecounts;
	int* heights;
	int width;
	int currentFrame;
	int currentAnimation;
};