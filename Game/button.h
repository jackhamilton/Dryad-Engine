#pragma once
#include <list>
#include <GameObject.h>
#include <SDL.h>

class Sprite;

class Button : public GameObject
{
public:
	Button(const char* text, const char* font, int fontSize, Point position, SDL_Color textColor);
	~Button();
private:
	std::list<Sprite> buttonStates;
};

