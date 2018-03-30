#include "Input.h"
#include <vector>
#include <map>
#include "GameLoop.h"

Input::Input()
{

}

//Adds a collection of pairs of keycodes and eventtypes matched with a function to call on that combo.
void Input::addKeyboardEvent(std::function<void()> func, std::vector<std::pair<SDL_Keycode, SDL_EventType>> keys)
{
	keyboardEventMap.insert(std::pair<std::vector<std::pair<SDL_Keycode, SDL_EventType>>, Callback>
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
