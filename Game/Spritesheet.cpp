#include "Spritesheet.h"

//framecounts is not zero indexed, just the number of frames
Spritesheet::Spritesheet(std::vector<char*> filename, std::vector<int> framecounts, std::vector<int> heights, int width, Scene* world): Sprite(filename, world)
{
    Spritesheet::heights = heights;
	Spritesheet::width = width;
	currentFrame = 0;
	currentAnimation = 0;
	Spritesheet::framecounts = framecounts;
	isSpritesheet = true;
}

//gives x and y
void Spritesheet::getCurrentFrame(int* dim)
{
	dim[0] = width * currentFrame; //startx
	int starty = 0;
	for (int x = 0; x < currentAnimation; x++) {
		starty += heights[x];
	}
	dim[1] = starty;
}

void Spritesheet::nextFrame()
{
	if (currentFrame < framecounts[currentAnimation] - 1) {
		currentFrame += 1;
	}
	else {
		currentFrame = 0;
	}
}

int Spritesheet::getWidth()
{
	return width;
}

int Spritesheet::getHeight()
{
	return heights[currentAnimation];
}
