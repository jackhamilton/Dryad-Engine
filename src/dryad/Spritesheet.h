#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include "Sprite.h"
#include <vector>

namespace dryad
{

class Renderer;

class Spritesheet : public Sprite {
public:
	Spritesheet();
    Spritesheet(std::vector<string> filename, std::vector<int> framecounts, std::vector<bool> looping, std::vector<int> heights, int width, int fps);
	void getCurrentFrame(std::shared_ptr<int> dimX, std::shared_ptr<int> dimY);
	bool isOnFinalFrame();
	void setCurrentAnimation(int currentAnimation) {
		if (currentAnimation != Spritesheet::currentAnimation) {
			Spritesheet::currentAnimation = currentAnimation;
			currentFrame = 0;
			paused = false;
		}
	}
	void nextImage();
	void render(std::shared_ptr<Renderer> renderer, Point locationMod);
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

}