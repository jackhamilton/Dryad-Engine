#pragma once
#include <SDL.h>
#include <thread>
#include <vector>
#include <map>
#include <functional>
#include "Mouse.h"
#include "Point.h"
#include "TextField.h"

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

//Mapping for linker
class GameLoop;

using namespace std;
class Input {
public:
	Input(shared_ptr<Mouse> mouse);
	void addKeyboardEvent(function<void()> func,vector<pair<SDL_Keycode, SDL_EventType>> keys);
	void handleInput(GameLoop* gameLoop);
	shared_ptr<Mouse> getMouse();
private:
	shared_ptr<Mouse> mouse;
	Point lButtonDown;
	bool isLButtonDown;
	Point rButtonDown;
	bool isRButtonDown;
	TextField* activeField = nullptr;
	bool textInputActive = false;
	bool lockSceneObjects = false;

	// Saves the state(true=pressed; false=released) of each SDL_Key
	std::map<SDL_Keycode, bool> keyboard; 

	std::map<vector<pair<SDL_Keycode, SDL_EventType>>, bool> keyUpExecuted;
	//a map of all keyboard events and functions to call
	std::map<vector<pair<SDL_Keycode, SDL_EventType>>, Callback> keyboardEventMap;
	friend class Scene;
};
