#include "Spritesheet.h"
#include "Renderer.h"

namespace dryad
{

Spritesheet::Spritesheet() : Sprite()
{
	loaded = false;
	paused = true;
	Spritesheet::heights = {};
	Spritesheet::width = 0;
	currentFrame = 0;
	currentAnimation = 0;
	Spritesheet::framecounts = {};
}

//framecounts is not zero indexed, just the number of frames
Spritesheet::Spritesheet(std::vector<std::string> filename, std::vector<int> framecounts, std::vector<bool> looping, std::vector<int> heights, int width, int fps) : Sprite(filename)
{
	loaded = false;
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
void Spritesheet::getCurrentFrame(std::shared_ptr<int> dimX, std::shared_ptr<int> dimY)
{
	*dimX = width * currentFrame; //startx
	int starty = 0;
	for (int x = 0; x < currentAnimation; x++) {
		starty += heights[x];
	}
	*dimY = starty;
}

bool Spritesheet::isOnFinalFrame()
{
	if (currentFrame >= framecounts[currentAnimation] - 1) {
		return true;
	}
	return false;
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

void Spritesheet::render(std::shared_ptr<Renderer> renderer, Point locationMod)
{
	std::shared_ptr<int> boxX = std::make_shared<int>();
	std::shared_ptr<int> boxY = std::make_shared<int>();
	getCurrentFrame(boxX, boxY);
	Point location = getLocation();
	SDL_Rect dsrect = { (int)location.x + (int)locationMod.x, (int)location.y + (int)locationMod.y, getWidth(), getHeight() };
	SDL_Rect srcrect = { *boxX, *boxY, getWidth(), getHeight() };
	renderer->render(getCurrentImage(), srcrect, dsrect);
}

}