#include "TextField.h"
#include "Line.h"

TextField::TextField(string text, string font, int fontSize, Rectangle position, shared_ptr<AssetLibrary> assetLibrary)
{
	TextField::font = font;
	TextField::currentText = text;
	TextField::fontSize = fontSize;
	TextField::position = Point(position.x, position.y);
	TextField::placement = position;
	assetLib = assetLibrary;
	isUIElement = true;
	isTextField = true;
	cSpritePointer = shared_ptr<CompositeSprite>(new CompositeSprite(1));
	createTextures(cSpritePointer);
	setSprite(cSpritePointer);
	hasMouseClickEvent = true;
	size = { position.width, position.height };
	
	mouseClickEvent = bind(&TextField::activate, this);
	onExitingScene = bind(&TextField::deactivate, this);
}

void TextField::setEnabled(bool enabled)
{
	if (TextField::enabled && selected && !enabled) {
		deactivate();
	}
	TextField::enabled = enabled;
}

void TextField::setText(string text)
{
	currentText = text;
	refreshTextures();
}

void TextField::activate() {
	if (!selected && enabled) {
		if (deactivateOtherCallback) {
			deactivateOtherCallback(this);
		}
		selected = true;
		refreshTextures();
		if (activateFieldCallback) {
			activateFieldCallback(this);
		}
	}
}

void TextField::refreshTextures() {

	auto cSprite = static_pointer_cast<CompositeSprite>(sprite);
	cSprite->reloadAllTextures();
	createTextures(cSprite);
}

void TextField::deactivate() {
	if (selected) {
		selected = false;
		auto cSprite = static_pointer_cast<CompositeSprite>(sprite);
		cSprite->reloadAllTextures();
		refreshTextures();
		if (activateFieldCallback) {
			activateFieldCallback(nullptr);
		}
	}
}

void TextField::backspace()
{
	if (placementPosition > 0 && placementPosition <= currentText.size() && currentText.size() > 1) {
		currentText.erase(currentText.begin() + placementPosition - 1);
		placementPosition--;
		refreshTextures();
	}
}

void TextField::insert(string s)
{
	if (placementPosition + 1 <= currentText.size() && s.size() > 0) {
		for (char c : s) {
			currentText.insert(currentText.begin() + placementPosition, c);
			placementPosition++;
		}
		refreshTextures();
	}
}

void TextField::createTextures(shared_ptr<CompositeSprite> s) {
	if (assetLib->fonts.find(font) == assetLib->fonts.end()
		|| assetLib->fonts[font].second != fontSize) {
		assetLib->openFont(font, fontSize);
	}
	SDL_Surface* surfaceMessage;
	int textWidth, textHeight;
	auto fontPtr = (assetLib->fonts[font]).first;
	if (TTF_SizeText(fontPtr, currentText.c_str(), &textWidth, &textHeight)) {
		printf(TTF_GetError());
	}
	if (!(surfaceMessage = TTF_RenderText_Blended((assetLib->fonts[font]).first, currentText.c_str(), { 0, 0, 0 }))) { // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		printf("Error rendering text in button.");
		string text = TTF_GetError();
		printf(text.c_str());
		//flags, width, height, depth ---- rmask, gmask, bmask, amask
		//https://wiki.libsdl.org/SDL_CreateRGBSurface

		surfaceMessage = SDL_CreateRGBSurface(0, getSize().width, getSize().height, 32, 0, 0, 0, 0);
	}
	SDL_Surface* bg = SDL_CreateRGBSurface(0, placement.width, placement.height, 32, 0, 0, 0, 0);
	SDL_Color fill = { 150, 150, 150 };
	if (selected) {
		fill.r = 255;
		fill.g = 255;
		fill.b = 255;
	}
	SDL_FillRect(bg, NULL, SDL_MapRGB(bg->format, fill.r, fill.g, fill.b));
	s->addSpriteFromSurfaces({ bg });
	int textHeightOffset = (placement.height - textHeight) / 2;
	xPlacementOffset = textHeightOffset;
	if (surfaceMessage) {
		s->addSpriteFromSurfaces({ surfaceMessage }, Point(textHeightOffset, textHeightOffset), textHeightOffset);
	}
	int cursorX = getCursorPosition();
	if (cursorX < placement.width - xPlacementOffset && selected) {
		pair<SDL_Surface*, Point> cursor = Line::drawLine(Point(cursorX, min(5, placement.height)), Point(cursorX, max(placement.height - 5, 0)), { 0, 0, 0 });
		s->addSpriteFromSurfaces({ cursor.first }, cursor.second, 0);
	}
	size = { placement.width, placement.height };
	//TODO add cursored version as separate sprite, animation engine should take care of it since it's 1fps already.
	//Requires compositesprite to be reworked to support more than one frame in loadtextures.
}

void TextField::updatePlacementPosition(Point p)
{
	vector<int> charWidths;
	charWidths.push_back(0);
	int textWidth, textHeight;
	auto fontPtr = (assetLib->fonts[font]).first;
	char c[2];
	for (int i = 0; i < currentText.size(); i++) {
		c[0] = currentText.c_str()[i];
		c[1] = '\0';
		TTF_SizeText(fontPtr, c, &textWidth, &textHeight);
		charWidths.push_back(textWidth + charWidths.back());
	}
	Point relativePosition = p - position;
	int testWidth = relativePosition.x - xPlacementOffset;
	//Not using binary search
	int minDiff = 100000;
	int placement = 0;
	for (int i = 0; i < charWidths.size(); i++) {
		int diff = abs(charWidths.at(i) - testWidth);
		if (diff < minDiff) {
			placement = i;
			minDiff = diff;
		}
	}
	placementPosition = placement;
	if (placementPosition > currentText.size()) {
		placementPosition = currentText.size();
	}
	refreshTextures();
}

int TextField::getCursorPosition()
{
	vector<int> charWidths;
	charWidths.push_back(0);
	int textWidth, textHeight;
	auto fontPtr = (assetLib->fonts[font]).first;
	char c[2];
	for (int i = 0; i < currentText.size(); i++) {
		c[0] = currentText.c_str()[i];
		c[1] = '\0';
		TTF_SizeText(fontPtr, c, &textWidth, &textHeight);
		charWidths.push_back(textWidth + charWidths.back());
	}
	return charWidths[placementPosition] + xPlacementOffset - 1;
}


void TextField::updatePlacementPosition(int mod) {
	placementPosition += mod;
	if (placementPosition > currentText.size()) {
		placementPosition = currentText.size();
	}
	if (placementPosition < 0) {
		placementPosition = 0;
	}
	refreshTextures();
}