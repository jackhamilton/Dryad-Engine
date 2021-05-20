#pragma once
#include <memory>
#include "GameObject.h"
#include "SDL.h"
class Line : public GameObject
{
public:
	Line(SDL_Color color, Point positionA, Point positionB);
	static pair<SDL_Surface*, Point> drawLine(Point positionA, Point positionB, SDL_Color color);
private:
	static void set_pixel(SDL_Surface* surface, int x, int y, Uint32 color);
};