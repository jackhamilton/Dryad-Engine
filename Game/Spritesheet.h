#pragma once
#include <SDL.h>
#include <Sprite.h>

class Spritesheet : public Sprite {
public:
	Spritesheet(const char* filename[], int* heights, int width, World* window);
	void getCurrentFrame(int* dim);
	void setCurrentAnimation(int currentAnimation) {
	    Spritesheet::currentAnimation = currentAnimation;
	}
	void destroy() {
		Sprite::destroy();
		delete heights;
	}
	void nextFrame();
	int getWidth();
	int getHeight();
private:
	int* heights;
	int width;
	int currentFrame;
	int currentAnimation;
};