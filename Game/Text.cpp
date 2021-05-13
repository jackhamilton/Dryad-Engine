#include "Text.h"
#include "SDL_ttf.h"

Text::Text(const char* text, const char* font, int fontSize, SDL_Color textColor, Point position) {
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
	if (!(surfaceMessage = TTF_RenderText_Solid(fontTTF, text, textColor))) { // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		printf("Error rendering text in button.");
		const char* text = TTF_GetError();
		printf(text);
		//flags, width, height, depth ---- rmask, gmask, bmask, amask
		//https://wiki.libsdl.org/SDL_CreateRGBSurface
		surfaceMessage = SDL_CreateRGBSurface(0, getSize().width, getSize().height, 32, 0, 0, 0, 0);
	}
	Sprite* s = new Sprite({ surfaceMessage });
	setSprite(s);
	Text::font = font;
	Text::fontSize = fontSize;
	Text::textColor = textColor;
	Text::position = position;
}

void Text::setText(const char* text)
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
	if (!(surfaceMessage = TTF_RenderText_Solid(fontTTF, text, textColor))) { // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		printf("Error rendering text in button.");
		const char* text = TTF_GetError();
		printf(text);
		//flags, width, height, depth ---- rmask, gmask, bmask, amask
		//https://wiki.libsdl.org/SDL_CreateRGBSurface
		surfaceMessage = SDL_CreateRGBSurface(0, getSize().width, getSize().height, 32, 0, 0, 0, 0);
		/* Filling the surface with red color. */
		SDL_FillRect(surfaceMessage, NULL, SDL_MapRGB(surfaceMessage->format, 255, 0, 0));
	}
	Sprite* s = new Sprite({ surfaceMessage });
	delete getSprite();
	setSprite(s);
}