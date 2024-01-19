#include "Button.h"
#include <CompositeSprite.h>

namespace dryad
{

//Include .ttf in font name
Button::Button(std::function<void()> onClick, std::string text, std::string font, int fontSize, SDL_Color textColor, std::shared_ptr<AssetLibrary> assetLibrary, SDL_Color backgroundColor, Point position, Size size) : GameObject(position)
{
	setSize(size);
	Button::text = text;
	Button::font = font;
	Button::fontSize = fontSize;
	std::shared_ptr<Sprite> bg = createTexture(textColor, backgroundColor, assetLibrary);
	setSprite(bg);
	buttonStates[0] = bg;
	mouseClickUpEvent = onClick;
	hasMouseClickUpEvent = true;
	Button::hasTextureData = true;
	hasHoverTexture = false;
	hasClickTexture = false;
	isUIElement = true;
}

Button::Button(std::function<void()> onClick, std::shared_ptr<Sprite> sprite, Point position, Size size) : GameObject(position)
{
	setSize(size);
	setSprite(sprite);
	buttonStates[0] = sprite;
	mouseClickUpEvent = onClick;
	hasMouseClickUpEvent = true;
	Button::hasTextureData = false;
	hasHoverTexture = false;
	hasClickTexture = false;
	isUIElement = true;
}

void Button::setHoverTexture(std::shared_ptr<Sprite> sprite) {
	if (!hasHoverTexture) {
		Button::buttonStates[1] = sprite;
		Button::hasHoverTexture = true;
		Button::mouseEnteredEvent = std::bind(&Button::mouseEntered, this);
		Button::hasMouseEnteredEvent = true;
		Button::mouseExitedEvent = std::bind(&Button::mouseExited, this);
		Button::hasMouseExitedEvent = true;
	} else {
		printf("ERROR: Button already has hover texture.\n");
	}
}

void Button::createHoverTexture(SDL_Color textColor, SDL_Color backgroundColor, std::shared_ptr<AssetLibrary> assetLibrary)
{
	if (!hasHoverTexture) {
		Button::buttonStates[1] = createTexture(textColor, backgroundColor, assetLibrary);
		Button::hasHoverTexture = true;
		Button::mouseEnteredEvent = std::bind(&Button::mouseEntered, this);
		Button::hasMouseEnteredEvent = true;
		Button::mouseExitedEvent = std::bind(&Button::mouseExited, this);
		Button::hasMouseExitedEvent = true;
	} else {
		printf("ERROR: Button already has hover texture.\n");
	}
}

void Button::setClickTexture(std::shared_ptr<Sprite> sprite) {
	if (!hasClickTexture) {
		Button::buttonStates[2] = sprite;
		Button::hasClickTexture = true;
		Button::mouseClickGraphicEvent = std::bind(&Button::mouseClickDown, this);
		Button::hasMouseClickGraphicEvent = true;
		Button::mouseClickUpGraphicEvent = std::bind(&Button::mouseClickUp, this);
		Button::hasMouseClickUpGraphicEvent = true;
	} else {
		printf("ERROR: Button already has click texture.\n");
	}
}

void Button::createClickTexture(SDL_Color textColor, SDL_Color backgroundColor, std::shared_ptr<AssetLibrary> assetLibrary)
{
	if (!hasClickTexture) {
		Button::buttonStates[2] = createTexture(textColor, backgroundColor, assetLibrary);
		Button::hasClickTexture = true;
		Button::mouseClickGraphicEvent = std::bind(&Button::mouseClickDown, this);
		Button::hasMouseClickGraphicEvent = true;
		Button::mouseClickUpGraphicEvent = std::bind(&Button::mouseClickUp, this);
		Button::hasMouseClickUpGraphicEvent = true;
	} else {
		printf("ERROR: Button already has click texture.\n");
	}
}

void Button::mouseEntered()
{
	if (hasHoverTexture) {
		setSprite(Button::buttonStates[1]);
	}
}

void Button::mouseExited()
{
	setSprite(Button::buttonStates[0]);
}

void Button::mouseClickDown()
{
	if (hasClickTexture) {
		setSprite(Button::buttonStates[2]);
	}
}

void Button::mouseClickUp()
{
	if (hasHoverTexture) {
		setSprite(Button::buttonStates[1]);
	}
	else {
		setSprite(Button::buttonStates[0]);
	}
}

std::shared_ptr<Sprite> Button::createTexture(SDL_Color textColor, SDL_Color backgroundColor, std::shared_ptr<AssetLibrary> assetLibrary)
{
	if (assetLibrary->fonts.find(font) == assetLibrary->fonts.end()
		|| assetLibrary->fonts[font].second != fontSize) {
		assetLibrary->openFont(font, fontSize);
	}
	SDL_Surface* surfaceMessage;
	int textWidth, textHeight;
	auto fontPtr = (assetLibrary->fonts[font]).first;
	if (TTF_SizeText(fontPtr, text.c_str(), &textWidth, &textHeight)) {
		printf(TTF_GetError());
	}
	if (!(surfaceMessage = TTF_RenderText_Blended((assetLibrary->fonts[font]).first, text.c_str(), textColor))) { // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		printf("Error rendering text in button.");
		std::string text = TTF_GetError();
		printf(text.c_str());
		//flags, width, height, depth ---- rmask, gmask, bmask, amask
		//https://wiki.libsdl.org/SDL_CreateRGBSurface
		surfaceMessage = SDL_CreateRGBSurface(0, getSize().width, getSize().height, 32, 0, 0, 0, 0);
	}
	SDL_Surface* bg = SDL_CreateRGBSurface(0, max(textWidth + 10, getSize().width), max(textHeight + 10, getSize().height), 32, 0, 0, 0, 0);
	SDL_FillRect(bg, NULL, SDL_MapRGB(bg->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
	std::shared_ptr<CompositeSprite> sprite = make_shared<CompositeSprite>(CompositeSprite());
	sprite->setCenterAll(true);
	sprite->addSpriteFromSurfaces({ bg });
	sprite->addSpriteFromSurfaces({ surfaceMessage });
	return sprite;
}

}