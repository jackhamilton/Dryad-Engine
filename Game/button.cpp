#include "Button.h"
#include "SDL_ttf.h"
#include <CompositeSprite.h>

//Include .ttf in font name
Button::Button(function<void()> onClick, const char* text, const char* font, int fontSize, SDL_Color textColor, SDL_Color backgroundColor, Point position, Size size) : GameObject(position)
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
	SDL_Surface *surfaceMessage;
	int textWidth, textHeight;
	if (TTF_SizeText(fontTTF, text, &textWidth, &textHeight)) {
		printf(TTF_GetError());
	}
	if (!(surfaceMessage = TTF_RenderText_Solid(fontTTF, text, textColor))) { // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		printf("Error rendering text in button.");
		const char* text = TTF_GetError();
		printf(text);
		//flags, width, height, depth ---- rmask, gmask, bmask, amask
		//https://wiki.libsdl.org/SDL_CreateRGBSurface
		surfaceMessage = SDL_CreateRGBSurface(0, size.width, size.height, 32, 0, 0, 0, 0);
		/* Filling the surface with red color. */
		SDL_FillRect(surfaceMessage, NULL, SDL_MapRGB(surfaceMessage->format, 255, 0, 0));
	}
	SDL_Surface *bg = SDL_CreateRGBSurface(0, max(textWidth + 10, size.width), max(textHeight + 10, size.height), 32, 0, 0, 0, 0);
	SDL_FillRect(bg, NULL, SDL_MapRGB(bg->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
	setSize(size);
	CompositeSprite* sprite = new CompositeSprite();
	sprite->setCenterAll(true);
	sprite->addSpriteFromSurfaces({ bg });
	sprite->addSpriteFromSurfaces({ surfaceMessage });
	hasSprite = true;
	setSprite(sprite);
	buttonStates[0] = sprite;
	mouseClickUpEvent = onClick;
	hasMouseClickUpEvent = true;
}

Button::Button(function<void()> onClick, Sprite* sprite, Point position, Size size) : GameObject(position)
{
	Button::setSize(size);
	hasSprite = true;
	setSprite(sprite);
	buttonStates[0] = sprite;
	mouseClickUpEvent = onClick;
	hasMouseClickUpEvent = true;
}

void Button::setHoverTexture(Sprite* sprite) {
	Button::buttonStates[1] = sprite;
}

void Button::setClickTexture(Sprite* sprite) {
	Button::buttonStates[2] = sprite;
}

//destructor
Button::~Button()
{
	
}