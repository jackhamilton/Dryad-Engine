#pragma once
#include <SDL.h>
#include <thread>
#include <vector>
#include <map>
#include <functional>
#include "Mouse.h"
#include "Point.h"
#include "TextField.h"

namespace dryad
{

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

//Mapping for linker
class GameLoop;

class Input {
public:
	Input(std::shared_ptr<Mouse> mouse);
	void addKeyboardEvent(function<void()> func,std::vector<std::pair<SDL_Keycode, SDL_EventType>> keys);
	void handleInput(GameLoop* gameLoop);
	std::shared_ptr<Mouse> getMouse();
private:
	std::shared_ptr<Mouse> mouse;
	Point lButtonDown;
	bool isLButtonDown;
	Point rButtonDown;
	bool isRButtonDown;
	TextField* activeField = nullptr;
	bool textInputActive = false;
	bool lockSceneObjects = false;

	// Saves the state(true=pressed; false=released) of each SDL_Key
	std::map<SDL_Keycode, bool> keyboard; 

	std::map<std::vector<std::pair<SDL_Keycode, SDL_EventType>>, bool> keyUpExecuted;
	//a map of all keyboard events and functions to call
	std::map<std::vector<std::pair<SDL_Keycode, SDL_EventType>>, Callback> keyboardEventMap;
	friend class Scene;
};
    
}
