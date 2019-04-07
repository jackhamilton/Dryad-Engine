#include "Button.h"
#include "SDL_ttf.h"
#include <CompositeSprite.h>

//Include .ttf in font name
Button::Button(const char* text, const char* font, int fontSize, Point position, SDL_Color textColor) : GameObject(position)
{
	char* fontName = _strdup(font);
	if (!TTF_WasInit()) {
		TTF_Init();
	}
	TTF_Font* Font;
	if (!(Font = TTF_OpenFont(strcat(SDL_GetBasePath(), fontName), fontSize))) { //this opens a font style and sets a size
		const char* text = TTF_GetError();
		printf(text);
	}
	SDL_Surface *surfaceMessage;
	if (!(surfaceMessage = TTF_RenderText_Solid(Font, text, textColor))) { // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		printf("Error rendering text in button.");
		const char* text = TTF_GetError();
		printf(text);
		surfaceMessage = SDL_CreateRGBSurface(0, 200, 200, 32, 0, 0, 0, 0);
		/* Filling the surface with red color. */
		SDL_FillRect(surfaceMessage, NULL, SDL_MapRGB(surfaceMessage->format, 255, 0, 0));
	}
	SDL_Surface *bg = SDL_CreateRGBSurface(0, 200, 100, 32, 0, 0, 0, 0);
	SDL_FillRect(bg, NULL, SDL_MapRGB(bg->format, 255, 0, 0));
	CompositeSprite* sprite = new CompositeSprite();
	sprite->addSpriteFromSurfaces({ bg });
	sprite->addSpriteFromSurfaces({ surfaceMessage });
	hasSprite = true;
	setSprite(sprite);
}

//destructor
Button::~Button()
{
	
}