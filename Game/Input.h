#pragma once
#include <SDL.h>
#include <thread>
#include <vector>
#include <map>
#include <functional>
#include "Mouse.h"
#include "Point.h"

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

//Mapping for linker
class GameLoop;

class Input {
public:
	Input(Mouse* mouse);
	void addKeyboardEvent(std::function<void()> func, std::vector<std::pair<SDL_Keycode, SDL_EventType>> keys);
	void handleInput(GameLoop* gameLoop);
	Mouse* getMouse();
private:
	Mouse* mouse;
	Point lButtonDown;
	bool isLButtonDown;
	Point rButtonDown;
	bool isRButtonDown;
	// Saves the state(true=pressed; false=released) of each SDL_Key
	std::map<SDL_Keycode, bool> keyboard; 

	std::map<std::vector<std::pair<SDL_Keycode, SDL_EventType>>, bool> keyUpExecuted;
	//a map of all keyboard events and functions to call
	std::map<std::vector<std::pair<SDL_Keycode, SDL_EventType>>, Callback> keyboardEventMap;
};
