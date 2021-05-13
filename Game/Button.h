#pragma once
#include <list>
#include <GameObject.h>
#include <SDL.h>
#include "SDL_ttf.h"
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
	void createHoverTexture(SDL_Color textColor, SDL_Color backgroundColor);
	void setClickTexture(Sprite* sprite);
	void createClickTexture(SDL_Color textColor, SDL_Color backgroundColor);
private:
	//0: base, 1: hover, 2: click
	const char* text;
	const char* font;
	int fontSize;
	bool hasTextureData;
	Sprite* buttonStates[3];
	int currentState;
	bool hasHoverTexture;
	bool hasClickTexture;
	void mouseEntered();
	void mouseExited();
	void mouseClickDown();
	void mouseClickUp();
	Sprite* createTexture(SDL_Color textColor, SDL_Color backgroundColor);
};