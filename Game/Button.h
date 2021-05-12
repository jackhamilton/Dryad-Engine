#pragma once
#include <list>
#include <GameObject.h>
#include <SDL.h>
#include <Size.h>
#include <functional>

class Sprite;

using namespace std;
class Button : public GameObject
{
public:
	Button(function<void()> onClick, Sprite* sprite, Point position, Size size);
	Button(function<void()> onClick, const char* text, const char* font, int fontSize, SDL_Color textColor, SDL_Color backgroundColor, Point position, Size size);
	//Initializes at text size plus padding
	Button(function<void()> onClick, const char* text, const char* font, int fontSize, SDL_Color textColor, SDL_Color backgroundColor, Point position) : Button(onClick, text, font, fontSize, textColor, backgroundColor, position, { 0, 0 }) {};
	~Button();
	void setHoverTexture(Sprite* sprite);
	void setClickTexture(Sprite* sprite);
private:
	//0: base, 1: hover, 2: click
	Sprite* buttonStates[3];
	int currentState;
	void triggerCallback(Point a, Point b);
};

