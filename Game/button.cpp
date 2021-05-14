#include "Button.h"
#include <CompositeSprite.h>

//Include .ttf in font name
Button::Button(function<void()> onClick, const char* text, const char* font, int fontSize, SDL_Color textColor, SDL_Color backgroundColor, Point position, Size size) : GameObject(position)
{
	setSize(size);
	Button::text = text;
	Button::font = font;
	Button::fontSize = fontSize;
	Sprite* bg = createTexture(textColor, backgroundColor);
	hasSprite = true;
	setSprite(bg);
	buttonStates[0] = bg;
	mouseClickUpEvent = onClick;
	hasMouseClickUpEvent = true;
	Button::hasTextureData = true;
}

Button::Button(function<void()> onClick, Sprite* sprite, Point position, Size size) : GameObject(position)
{
	setSize(size);
	hasSprite = true;
	setSprite(sprite);
	buttonStates[0] = sprite;
	mouseClickUpEvent = onClick;
	hasMouseClickUpEvent = true;
	Button::hasTextureData = false;
}

void Button::setHoverTexture(Sprite* sprite) {
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

void Button::createHoverTexture(SDL_Color textColor, SDL_Color backgroundColor)
{
	if (!hasHoverTexture) {
		Button::buttonStates[1] = createTexture(textColor, backgroundColor);
		Button::hasHoverTexture = true;
		Button::mouseEnteredEvent = std::bind(&Button::mouseEntered, this);
		Button::hasMouseEnteredEvent = true;
		Button::mouseExitedEvent = std::bind(&Button::mouseExited, this);
		Button::hasMouseExitedEvent = true;
	} else {
		printf("ERROR: Button already has hover texture.\n");
	}
}

void Button::setClickTexture(Sprite* sprite) {
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

void Button::createClickTexture(SDL_Color textColor, SDL_Color backgroundColor)
{
	if (!hasClickTexture) {
		Button::buttonStates[2] = createTexture(textColor, backgroundColor);
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

Sprite* Button::createTexture(SDL_Color textColor, SDL_Color backgroundColor)
{
	char* fontName = _strdup(font);
	if (!TTF_WasInit()) {
		TTF_Init();
	}
	TTF_Font* fontTTF;
	if (!(fontTTF = TTF_OpenFont(strcat(SDL_GetBasePath(), fontName), fontSize))) { //this opens a font style and sets a size
		const char* text = TTF_GetError();
		printf(text);
	}
	SDL_Surface* surfaceMessage;
	int textWidth, textHeight;
	if (TTF_SizeText(fontTTF, text, &textWidth, &textHeight)) {
		printf(TTF_GetError());
	}
	if (!(surfaceMessage = TTF_RenderText_Blended(fontTTF, text, textColor))) { // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		printf("Error rendering text in button.");
		const char* text = TTF_GetError();
		printf(text);
		//flags, width, height, depth ---- rmask, gmask, bmask, amask
		//https://wiki.libsdl.org/SDL_CreateRGBSurface
		surfaceMessage = SDL_CreateRGBSurface(0, getSize().width, getSize().height, 32, 0, 0, 0, 0);
		/* Filling the surface with red color. */
		SDL_FillRect(surfaceMessage, NULL, SDL_MapRGB(surfaceMessage->format, 255, 0, 0));
	}
	SDL_Surface* bg = SDL_CreateRGBSurface(0, max(textWidth + 10, getSize().width), max(textHeight + 10, getSize().height), 32, 0, 0, 0, 0);
	SDL_FillRect(bg, NULL, SDL_MapRGB(bg->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
	CompositeSprite* sprite = new CompositeSprite();
	sprite->setCenterAll(true);
	sprite->addSpriteFromSurfaces({ bg });
	sprite->addSpriteFromSurfaces({ surfaceMessage });
	return sprite;
}

//destructor
Button::~Button()
{
	
}