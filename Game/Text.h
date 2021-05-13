#pragma once
#include "GameObject.h"
#include "SDL.h"
#include "SDL_ttf.h"

class Text : public GameObject
{
public:
	Text(const char* text, const char* font, int fontSize, SDL_Color textColor, Point position);
	void setText(const char* text);
private:
	const char* font;
	int fontSize;
	SDL_Color textColor;
};