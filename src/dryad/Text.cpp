#include "Text.h"

namespace dryad
{

Text::Text(std::string text, std::string font, int fontSize, SDL_Color textColor, Point position, std::shared_ptr<AssetLibrary> assetLibrary) {
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
	std::shared_ptr<Sprite> s (new Sprite({ surfaceMessage }));
	setSprite(s);
	Text::font = font;
	Text::fontSize = fontSize;
	Text::textColor = textColor;
	Text::position = position;
	isUIElement = true;
}

void Text::setText(std::string text, std::shared_ptr<AssetLibrary> assetLibrary)
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
	std::shared_ptr<Sprite> s(new Sprite({ surfaceMessage }));
	renderQueue.clear();
	setSprite(s);
}

}
