#pragma once
#include <list>
#include <GameObject.h>
#include <SDL.h>
#include "SDL_ttf.h"
#include <Size.h>
#include <AssetLibrary.h>
#include <functional>
#include <memory>
#include <string>

class Sprite;

using namespace std;
class Button : public GameObject
{
public:
	Button(function<void()> onClick, shared_ptr<Sprite> sprite, Point position, Size size);
	Button(function<void()> onClick, string text, string font, int fontSize, SDL_Color textColor, shared_ptr<AssetLibrary> assetLibrary, SDL_Color backgroundColor, Point position, Size size);
	//Initializes at text size plus padding
	Button(function<void()> onClick, string text, string font, int fontSize, SDL_Color textColor, shared_ptr<AssetLibrary> assetLibrary, SDL_Color backgroundColor, Point position) : Button(onClick, text, font, fontSize, textColor, assetLibrary, backgroundColor, position, { 0, 0 }) {};
	void setHoverTexture(shared_ptr<Sprite> sprite);
	void createHoverTexture(SDL_Color textColor, SDL_Color backgroundColor, shared_ptr<AssetLibrary> assetLibrary);
	void setClickTexture(shared_ptr<Sprite> sprite);
	void createClickTexture(SDL_Color textColor, SDL_Color backgroundColor, shared_ptr<AssetLibrary> assetLibrary);
private:
	//0: base, 1: hover, 2: click
	string text;
	string font;
	int fontSize;
	bool hasTextureData;
	shared_ptr<Sprite> buttonStates[3];
	int currentState;
	bool hasHoverTexture;
	bool hasClickTexture;
	void mouseEntered();
	void mouseExited();
	void mouseClickDown();
	void mouseClickUp();
	shared_ptr<Sprite> createTexture(SDL_Color textColor, SDL_Color backgroundColor, shared_ptr<AssetLibrary> assetLibrary);
};