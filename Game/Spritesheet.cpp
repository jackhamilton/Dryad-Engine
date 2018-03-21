#include "Spritesheet.h"

Spritesheet::Spritesheet(const char* filename[], int* heights, int width, Window* window): Sprite(filename, window)
{
	Spritesheet::heights = new int[sizeof(heights)/sizeof(heights[0])];
	for (int x = 0; x < sizeof(heights) / sizeof(heights[0]); x++) {
		Spritesheet::heights[x] = heights[x];
	}
	Spritesheet::width = width;
	delete heights;
	currentFrame = 0;
	currentAnimation = 0;
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
	if (currentFrame * width < Sprite::getDimensions().first) {
		currentFrame += 1;
	}
	else {
		currentFrame = 0;
	}
}
