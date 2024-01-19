#pragma once
#include <memory>
#include <string>
#include "GameObject.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "AssetLibrary.h"
#include "CompositeSprite.h"


namespace dryad
{

class TextField : public GameObject {
public:
	TextField(string text, string font, int fontSize, Rectangle position, shared_ptr<AssetLibrary> assetLibrary);
	string getText() {
		return currentText;
	}
	void setEnabled(bool enabled);
	void setText(string text);
private:
	void activate();
	void refreshTextures();
	void deactivate();
	void backspace();
	void insert(string s);
	void createTextures(shared_ptr<CompositeSprite> s);
	void updatePlacementPosition(Point p);
	int getCursorPosition();
	void updatePlacementPosition(int mod);
	int placementPosition = 0;
	int xPlacementOffset = 0;
	shared_ptr<CompositeSprite> cSpritePointer;
	bool selected = false;
	bool enabled = true;
	function<void(TextField*)> deactivateOtherCallback;
	function<void(TextField*)> activateFieldCallback;
	shared_ptr<AssetLibrary> assetLib;
	string currentText;
	string font;
	Rectangle placement;
	int fontSize;
	friend class Scene;
	friend class Input;
};

}