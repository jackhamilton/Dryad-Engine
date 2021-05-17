#pragma once
#include <memory>
#include <string>
#include "GameObject.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "AssetLibrary.h"

class Text : public GameObject
{
public:
	Text(string text, string font, int fontSize, SDL_Color textColor, Point position, shared_ptr<AssetLibrary> assetLibrary);
	void setText(string text, shared_ptr<AssetLibrary> assetLibrary);
private:
	string font;
	int fontSize;
	SDL_Color textColor;
};