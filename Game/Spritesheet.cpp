#include "Spritesheet.h"
#include "Renderer.h"

Spritesheet::Spritesheet() : Sprite()
{
	paused = true;
	Spritesheet::heights = {};
	Spritesheet::width = 0;
	currentFrame = 0;
	currentAnimation = 0;
	Spritesheet::framecounts = {};
}

//framecounts is not zero indexed, just the number of frames
Spritesheet::Spritesheet(std::vector<const char*> filename, std::vector<int> framecounts, std::vector<bool> looping, std::vector<int> heights, int width, int fps): Sprite(filename)
{
    Spritesheet::heights = heights;
	Spritesheet::width = width;
	currentFrame = 0;
	currentAnimation = 0;
	Spritesheet::looping = looping;
	Sprite::size = { width, heights[0] };
	Spritesheet::framecounts = framecounts;
	Sprite::fps = fps;
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

void Spritesheet::nextImage()
{
	if (!paused) {
		if (currentFrame < framecounts[currentAnimation] - 1) {
			currentFrame += 1;
		}
		else {
			currentFrame = 0;
			if (!looping[currentAnimation]) {
				paused = true;
			}
		}
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

std::pair<int, int> Spritesheet::getDimensions()
{
	return std::pair<int, int>(getWidth(), getHeight());
}

void Spritesheet::render(Renderer* renderer, Point locationMod)
{
	int* box = new int[2];
	getCurrentFrame(box);
	Point location = getLocation();
	SDL_Rect dsrect = { (int)location.x + (int)locationMod.x, (int)location.y + (int)locationMod.y, getWidth(), getHeight() };
	renderer->render(getCurrentImage(),
		{ box[0], box[1], getWidth(), getHeight() }, dsrect);
}