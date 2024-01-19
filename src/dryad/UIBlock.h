#pragma once
#include "GameObject.h"
#include <memory>
#include "HorizontalStackFrame.h"
#include "VerticalStackFrame.h"

namespace dryad
{

class UIBlock : public GameObject
{
public:
	UIBlock(Rectangle r, SDL_Color backgroundColor);
	UIBlock(Point p, Size s, SDL_Color backgroundColor);
private:
	void align(); 
	SDL_Surface* createTexture();
	SDL_Color backgroundColor;
};

}
