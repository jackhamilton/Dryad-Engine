#include "Input.h"
#include <vector>
#include <map>

Input::Input()
{

}

void Input::addKeyboardEvent(std::function<void()> func, SDL_EventType eventType, std::vector<SDL_Keycode> keys)
{
	for (SDL_Keycode key : keys) {
		keyboardEventMap.insert(std::pair<std::pair<SDL_Keycode, SDL_EventType>, Callback>
			(std::pair<SDL_Keycode, SDL_EventType>(key, eventType), (Callback)func));
	}
}

void Input::handleInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{	
		//React to each event in eventMap
		for (auto const& x : keyboardEventMap)
		{
			if (event.type == x.first.second && event.key.keysym.sym == x.first.first) {
				x.second();
				//Call the associated function if types are equal
			}
		}
	}
}
