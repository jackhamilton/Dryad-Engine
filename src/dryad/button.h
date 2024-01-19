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

namespace dryad
{

class Sprite;

class Button : public GameObject
{
public:
	Button(std::function<void()> onClick, std::shared_ptr<Sprite> sprite, Point position, Size size);
	Button(std::function<void()> onClick, std::string text, std::string font, int fontSize, SDL_Color textColor, std::shared_ptr<AssetLibrary> assetLibrary, SDL_Color backgroundColor, Point position, Size size);
	//Initializes at text size plus padding
	Button(std::function<void()> onClick, std::string text, std::string font, int fontSize, SDL_Color textColor, std::shared_ptr<AssetLibrary> assetLibrary, SDL_Color backgroundColor, Point position) : Button(onClick, text, font, fontSize, textColor, assetLibrary, backgroundColor, position, { 0, 0 }) {};
	void setHoverTexture(std::shared_ptr<Sprite> sprite);
	void createHoverTexture(SDL_Color textColor, SDL_Color backgroundColor, std::shared_ptr<AssetLibrary> assetLibrary);
	void setClickTexture(std::shared_ptr<Sprite> sprite);
	void createClickTexture(SDL_Color textColor, SDL_Color backgroundColor, std::shared_ptr<AssetLibrary> assetLibrary);
private:
	//0: base, 1: hover, 2: click
	std::string text;
	std::string font;
	int fontSize;
	bool hasTextureData;
	std::shared_ptr<Sprite> buttonStates[3];
	int currentState;
	bool hasHoverTexture;
	bool hasClickTexture;
	void mouseEntered();
	void mouseExited();
	void mouseClickDown();
	void mouseClickUp();
	std::shared_ptr<Sprite> createTexture(SDL_Color textColor, SDL_Color backgroundColor, std::shared_ptr<AssetLibrary> assetLibrary);
};

}