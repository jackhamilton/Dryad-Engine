#include "Input.h"
#include <vector>
#include <map>
#include <functional>
#include "GameLoop.h"
#include "Point.h"
#include "Rectangle.h"

using namespace std;
Input::Input(Mouse* mouse)
{
	Input::mouse = mouse;
	isLButtonDown = false;
	isRButtonDown = false;
}

//Adds a collection of pairs of keycodes and eventtypes matched with a function to call on that combo.
void Input::addKeyboardEvent(function<void()> func, vector<pair<SDL_Keycode, SDL_EventType>> keys)
{
	keyboardEventMap.insert(pair<vector<pair<SDL_Keycode, SDL_EventType>>, Callback>
		(keys, (Callback)func));
} 

void Input::handleInput(GameLoop* gameLoop)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{	
		switch (event.type) {
		case SDL_KEYDOWN:
			keyboard[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			keyboard[event.key.keysym.sym] = false;
			//React to KeyUp events
			for (auto const& x : keyboardEventMap)
			{
				bool execute = true;
				for (std::pair<SDL_Keycode, SDL_EventType> keycombo : x.first) {
					//If it's not a keyup or is both in the keyboard and true
					if ((keyboard.find(keycombo.first) != keyboard.end()
						&& keyboard.at(keycombo.first))
						|| keycombo.second != SDL_KEYUP) {
						execute = false;
					}
				}
				if (x.first.size() == 0) {
					execute = false;
				}
				//If it is pressed and either isn't in the map (first call) or hasn't been executed yet
				if (execute
					&& (keyUpExecuted.find(x.first) == keyUpExecuted.end()
						|| !keyUpExecuted.at(x.first))) {
					//Call the associated function if all keys matched. 
					//Keyup functions will always call if all keys are up.
					x.second();
					keyUpExecuted[x.first] = true;
				}
			}
			break;

		case SDL_MOUSEMOTION:
			int x, y;
			SDL_GetMouseState(&x, &y);
			mouse->position.x = x;
			mouse->position.y = y;
			for (function<void(Point)> func : mouse->mouseMovementEvents) {
				//Will have to create point every time. Perhaps change to int pair? would be very marginally more efficient
				func(Point(x, y));
			}
			if (mouse->activeScene) {
				vector<pair<function<void()>, Rectangle>> vec = *mouse->sceneMouseMovementEvents;
				for (vector<pair<function<void()>, Rectangle>>::iterator it = vec.begin(); it != vec.end(); it++)
				{
					pair<function<void()>, Rectangle> pair = *it;
					function<void()> func = pair.first;
					Rectangle activeRect = pair.second;
					if (x > activeRect.x and x < activeRect.x + activeRect.width and y > activeRect.y and y < activeRect.y + activeRect.height) {
						func();
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (function<void(Point)> func : mouse->mouseClickEvents) {
					func(Point(x, y));
				}
				if (mouse->activeScene) {
					vector<pair<function<void()>, Rectangle>> vec = *mouse->sceneMouseClickEvents;
					for (vector<pair<function<void()>, Rectangle>>::iterator it = vec.begin(); it != vec.end(); it++)
					{
						pair<function<void()>, Rectangle> pair = *it;
						function<void()> func = pair.first;
						Rectangle activeRect = pair.second;
						if (x > activeRect.x and x < activeRect.x + activeRect.width and y > activeRect.y and y < activeRect.y + activeRect.height) {
							func();
						}
					}
				}
				isLButtonDown = true;
				lButtonDown.x = x;
				lButtonDown.y = y;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (function<void(Point)> func : mouse->mouseRightClickEvents) {
					func(Point(x, y));
				}
				if (mouse->activeScene) {
					vector<pair<function<void()>, Rectangle>> vec = *mouse->sceneMouseRightClickEvents;
					for (vector<pair<function<void()>, Rectangle>>::iterator it = vec.begin(); it != vec.end(); it++)
					{
						pair<function<void()>, Rectangle> pair = *it;
						function<void()> func = pair.first;
						Rectangle activeRect = pair.second;
						if (x > activeRect.x and x < activeRect.x + activeRect.width and y > activeRect.y and y < activeRect.y + activeRect.height) {
							func();
						}
					}
				}
				isRButtonDown = true;
				rButtonDown.x = x;
				rButtonDown.y = y;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				Input::isLButtonDown = false;
				for (function<void(Point, Point)> func : mouse->mouseClickUpEvents) {
					func(Input::lButtonDown, Point(x, y));
				}
				if (mouse->activeScene) {
					vector<pair<function<void()>, Rectangle>> vec = *mouse->sceneMouseClickUpEvents;
					for (vector<pair<function<void()>, Rectangle>>::iterator it = vec.begin(); it != vec.end(); it++)
					{
						pair<function<void()>, Rectangle> pair = *it;
						function<void()> func = pair.first;
						Rectangle activeRect = pair.second;
						if ((x > activeRect.x and x < activeRect.x + activeRect.width and y > activeRect.y and y < activeRect.y + activeRect.height) and
							(lButtonDown.x > activeRect.x and lButtonDown.x < activeRect.x + activeRect.width and lButtonDown.y > activeRect.y and lButtonDown.y < activeRect.y + activeRect.height)) {
							func();
						}
					}
				}
				Input::lButtonDown.x = 0;
				Input::lButtonDown.y = 0;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				Input::isRButtonDown = false;
				for (function<void(Point, Point)> func : mouse->mouseRightClickUpEvents) {
					func(Input::rButtonDown, Point(x, y));
				}
				if (mouse->activeScene) {
					vector<pair<function<void()>, Rectangle>> vec = *mouse->sceneMouseRightClickUpEvents;
					for (vector<pair<function<void()>, Rectangle>>::iterator it = vec.begin(); it != vec.end(); it++)
					{
						pair<function<void()>, Rectangle> pair = *it;
						function<void()> func = pair.first;
						Rectangle activeRect = pair.second;
						if ((x > activeRect.x and x < activeRect.x + activeRect.width and y > activeRect.y and y < activeRect.y + activeRect.height) and
							(lButtonDown.x > activeRect.x and lButtonDown.x < activeRect.x + activeRect.width and lButtonDown.y > activeRect.y and lButtonDown.y < activeRect.y + activeRect.height)) {
							func();
						}
					}
				}
				Input::rButtonDown.x = 0;
				Input::rButtonDown.y = 0;
			}
			break;
		case SDL_QUIT:
			gameLoop->stop();
			break;
		}
	}
	//React to KeyDown events
	for (auto const& x : keyboardEventMap)
	{
		bool execute = true;
		for (std::pair<SDL_Keycode, SDL_EventType> keycombo : x.first) {
			//If the key hasn't been pressed - e.g. not in the keyboard map or false
			if (keyboard.find(keycombo.first) == keyboard.end()
				|| !keyboard.at(keycombo.first)) {
				//if (keycombo.second == SDL_KEYDOWN) {
					execute = false;
				//}
			}
			//if it's in the map and pressed, and the selected combo is a keyup1
			else if (keycombo.second == SDL_KEYUP) {
				//reset the selected combo
				keyUpExecuted[x.first] = false;
			}
		}
		//Reset any keyup combos containing pressed keys

		if (x.first.size() == 0) {
			execute = false;
		}
		if (execute) {
			//Call the associated function if all keys matched. 
			//For keyup there should really only ever be one.
			x.second();
		}
	}
}

Mouse* Input::getMouse()
{
	return mouse;
}
