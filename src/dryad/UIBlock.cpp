#include "UIBlock.h"

namespace dryad
{

UIBlock::UIBlock(Rectangle r, SDL_Color backgroundColor) {
	UIBlock::backgroundColor = backgroundColor;
	setPosition(Point(r.x, r.y));
	setSize({ r.width, r.height });
	SDL_Surface* bg = createTexture();
	auto sprite = std::shared_ptr<Sprite>(new Sprite({ bg }));
	setSprite(sprite);
	isUIElement = true;
}

UIBlock::UIBlock(Point p, Size s, SDL_Color backgroundColor)
{
	UIBlock::backgroundColor = backgroundColor;
	setPosition(p);
	setSize(s);
	SDL_Surface* bg = createTexture();
	auto sprite = std::shared_ptr<Sprite>(new Sprite({ bg }));
	setSprite(sprite);
	isUIElement = true;
}

void UIBlock::align()
{
	int totalStackHeight = size.height, totalStackWidth = size.width;
	int vStacks = 0, hStacks = 0;
	for (std::shared_ptr<GameObject> g : children) {
		totalStackHeight -= g->size.height;
		totalStackWidth -= g->size.width;
		if (g->isHorizontalStackFrame) {
			hStacks++;
		}
		else if (g->isVerticalStackFrame) {
			vStacks++;
		}
	}
	for (std::shared_ptr<GameObject> g : children) {
		if (g->isHorizontalStackFrame || g->isVerticalStackFrame) {
			g->size.height = totalStackHeight / (vStacks + hStacks);
			g->size.width = totalStackWidth / (vStacks + hStacks);
			static_pointer_cast<StackFrame>(g)->align();
		}
	}
}

SDL_Surface* UIBlock::createTexture()
{
	SDL_Surface* bg = SDL_CreateRGBSurface(0, getSize().width, getSize().height, 32, 0, 0, 0, 0);
	SDL_FillRect(bg, NULL, SDL_MapRGB(bg->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
	return bg;
}

}

