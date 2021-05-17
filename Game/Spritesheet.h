#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include "Sprite.h"
#include <vector>

class Renderer;

using namespace std;
class Spritesheet : public Sprite {
public:
	Spritesheet();
    Spritesheet(vector<string> filename, vector<int> framecounts, vector<bool> looping, vector<int> heights, int width, int fps);
	void getCurrentFrame(shared_ptr<int> dimX, shared_ptr<int> dimY);
	bool isOnFinalFrame();
	void setCurrentAnimation(int currentAnimation) {
		if (currentAnimation != Spritesheet::currentAnimation) {
			Spritesheet::currentAnimation = currentAnimation;
			currentFrame = 0;
			paused = false;
		}
	}
	void nextImage();
	void render(shared_ptr<Renderer> renderer, Point locationMod);
	int getWidth();
	int getHeight();
	pair<int, int> getDimensions();
private:
    vector<int> framecounts;
    vector<int> heights;
	vector<bool> looping;
	int width;
	int currentFrame;
	int currentAnimation;
};