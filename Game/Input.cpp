#include "Input.h"
#include <vector>
#include <map>
#include <functional>
#include "GameLoop.h"
#include "Point.h"
#include "Rectangle.h"

using namespace std;
Input::Input(shared_ptr<Mouse> mouse)
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

//TODO: click events do not work on moving gameobjects due to passing a rectangle as callback. Change to GameObject* or Rectangle*.
void Input::handleInput(GameLoop* gameLoop)
{
	if (activeField && !textInputActive) {
		SDL_StartTextInput();
		textInputActive = true;
	}
	else if (!activeField && textInputActive) {
		SDL_StopTextInput();
		textInputActive = false;
	}
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{	
		switch (event.type) {
		case SDL_TEXTINPUT:
			if (activeField) {
				//not copy or pasting
				if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V')))
				{
					activeField->insert(event.text.text);
				}
			}
			break;
		case SDL_KEYDOWN:
			keyboard[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			keyboard[event.key.keysym.sym] = false;
			//React to KeyUp events
			if (activeField) {
				if (event.key.keysym.sym == SDLK_LEFT) {
					activeField->updatePlacementPosition(-1);
				}
				else if (event.key.keysym.sym == SDLK_RIGHT) {
					activeField->updatePlacementPosition(1);
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE) {
					activeField->backspace();
				}
				else if (event.key.keysym.sym == SDLK_RETURN) {
					activeField->deactivate();
				}
				else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
					activeField->insert(SDL_GetClipboardText());
				}
			}
			else {
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
			}
			break;

		case SDL_MOUSEMOTION:
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			Point activePoint = Point(x, y);
			for (function<void(Point)> func : mouse->mouseMovementEvents) {
				func(activePoint);
			}
			if (mouse->activeScene && mouse->sceneObjectSet) {
				auto objects = mouse->sceneObjectSet;
				auto it = objects->begin();
				vector<Rectangle> invalidRects;
				for (; it != objects->end(); it++) {
					auto it2 = it->second.begin();
					for (; it2 != it->second.end(); it2++) {
						vector<shared_ptr<GameObject>> objectAndChildren;
						objectAndChildren.push_back(*it2);
						for (shared_ptr<GameObject> g : (*it2)->getChildrenFlat()) {
							objectAndChildren.push_back(g);
						}
						for (shared_ptr<GameObject> o : objectAndChildren) {
							bool invalidate = false;
							for (Rectangle r : invalidRects) {
								if (r.isInside(activePoint)) {
									invalidate = true;
								}
							}
							if (!invalidate && o->getActiveRectangle().isInside(activePoint)) {
								if (o->hasMouseMoveEvent) {
									o->mouseMoveEvent();
								}
								if (o->hasMouseEnteredEvent) {
									o->mouseEnteredEvent();
								}
							}
							else if (o->hasMouseExitedEvent && o->getActiveRectangle().isInside(mouse->position)) {
								//invalidation has to be recalculated based on previous mouse position.
								//this one doesn't matter if it's in an invalid rect. We only have to know if prev position was valid.
								invalidate = false;
								for (Rectangle r : invalidRects) {
									if (r.isInside(mouse->position)) {
										invalidate = true;
									}
								}
								if (!invalidate) {
									o->mouseExitedEvent();
								}
							}
							invalidRects.push_back(o->getActiveRectangle());
						}
					}
				}
			}
			mouse->position.x = x;
			mouse->position.y = y;
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				Point activePoint = Point(x, y);
				for (function<void(Point)> func : mouse->mouseClickEvents) {
					func(Point(x, y));
				}
				bool fieldLocksInput = false;
				if (activeField) {
					Rectangle bounds = activeField->placement;
					bounds.x = activeField->getPosition().x;
					bounds.x = activeField->getPosition().y;
					if (!(x > bounds.x and x < bounds.x + bounds.width and y > bounds.y and y < bounds.y + bounds.height)) {
						activeField->deactivate();
					}
					else {
						//This click activated the field, or was on an active field
						activeField->updatePlacementPosition(Point(x, y));
						fieldLocksInput = true;
					}
				}
				if (!fieldLocksInput && mouse->activeScene && mouse->sceneObjectSet) {
					auto objects = mouse->sceneObjectSet;
					auto it = objects->begin();
					vector<Rectangle> invalidRects;
					for (; it != objects->end(); it++) {
						auto it2 = it->second.begin();
						for (; it2 != it->second.end(); it2++) {
							vector<shared_ptr<GameObject>> objectAndChildren;
							objectAndChildren.push_back(*it2);
							for (shared_ptr<GameObject> g : (*it2)->getChildrenFlat()) {
								objectAndChildren.push_back(g);
							}
							for (shared_ptr<GameObject> o : objectAndChildren) {
								bool invalidate = false;
								for (Rectangle r : invalidRects) {
									if (r.isInside(activePoint)) {
										invalidate = true;
									}
								}
								if (!invalidate && o->getActiveRectangle().isInside(activePoint)) {
									if (o->hasMouseClickEvent) {
										o->mouseClickEvent();
									}
									else if (o->hasMouseClickGraphicEvent) {
										o->mouseClickGraphicEvent();
									}
								}
								invalidRects.push_back(o->getActiveRectangle());
							}
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
				Point activePoint = Point(x, y);
				for (function<void(Point)> func : mouse->mouseRightClickEvents) {
					func(Point(x, y));
				}
				if (mouse->activeScene && mouse->sceneObjectSet) {
					auto objects = mouse->sceneObjectSet;
					auto it = objects->begin();
					vector<Rectangle> invalidRects;
					for (; it != objects->end(); it++) {
						auto it2 = it->second.begin();
						for (; it2 != it->second.end(); it2++) {
							vector<shared_ptr<GameObject>> objectAndChildren;
							objectAndChildren.push_back(*it2);
							for (shared_ptr<GameObject> g : (*it2)->getChildrenFlat()) {
								objectAndChildren.push_back(g);
							}
							for (shared_ptr<GameObject> o : objectAndChildren) {
								bool invalidate = false;
								for (Rectangle r : invalidRects) {
									if (r.isInside(activePoint)) {
										invalidate = true;
									}
								}
								if (!invalidate && o->getActiveRectangle().isInside(activePoint)) {
									if (o->hasMouseRightClickEvent) {
										o->mouseRightClickEvent();
									}
								}
								invalidRects.push_back(o->getActiveRectangle());
							}
						}
					}
				}
				isRButtonDown = true;
				rButtonDown.x = x;
				rButtonDown.y = y;
			}
		}
		break;
		case SDL_MOUSEBUTTONUP:
		{
			if (event.button.button == SDL_BUTTON_LEFT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				Point activePoint = Point(x, y);
				for (function<void(Point, Point)> func : mouse->mouseClickUpEvents) {
					func(Input::lButtonDown, Point(x, y));
				}
				bool fieldLocksInput = false;
				if (activeField) {
					Rectangle bounds = activeField->placement;
					bounds.x = activeField->getPosition().x;
					bounds.x = activeField->getPosition().y;
					if ((x > bounds.x and x < bounds.x + bounds.width and y > bounds.y and y < bounds.y + bounds.height)) {
						fieldLocksInput = true;
					}
				}
				if (!fieldLocksInput && mouse->activeScene && mouse->sceneObjectSet) {
					auto objects = mouse->sceneObjectSet;
					auto it = objects->begin();
					vector<Rectangle> invalidRects;
					for (; it != objects->end(); it++) {
						auto it2 = it->second.begin();
						for (; it2 != it->second.end(); it2++) {
							vector<shared_ptr<GameObject>> objectAndChildren;
							objectAndChildren.push_back(*it2);
							for (shared_ptr<GameObject> g : (*it2)->getChildrenFlat()) {
								objectAndChildren.push_back(g);
							}
							for (shared_ptr<GameObject> o : objectAndChildren) {
								bool invalidate = false;
								for (Rectangle r : invalidRects) {
									if (r.isInside(activePoint)) {
										invalidate = true;
									}
								}
								if (!invalidate && o->getActiveRectangle().isInside(activePoint)) {
									if (o->hasMouseClickUpEvent) {
										o->mouseClickUpEvent();
									}
									else if (o->hasMouseClickUpGraphicEvent) {
										o->mouseClickUpGraphicEvent();
									}
								}
								invalidRects.push_back(o->getActiveRectangle());
							}
						}
					}
				}
				Input::isLButtonDown = false;
				Input::lButtonDown.x = 0;
				Input::lButtonDown.y = 0;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				Point activePoint = Point(x, y);
				for (function<void(Point, Point)> func : mouse->mouseRightClickUpEvents) {
					func(Input::rButtonDown, Point(x, y));
				}
				if (mouse->activeScene && mouse->sceneObjectSet) {
					auto objects = mouse->sceneObjectSet;
					auto it = objects->begin();
					vector<Rectangle> invalidRects;
					for (; it != objects->end(); it++) {
						auto it2 = it->second.begin();
						for (; it2 != it->second.end(); it2++) {
							vector<shared_ptr<GameObject>> objectAndChildren;
							objectAndChildren.push_back(*it2);
							for (shared_ptr<GameObject> g : (*it2)->getChildrenFlat()) {
								objectAndChildren.push_back(g);
							}
							for (shared_ptr<GameObject> o : objectAndChildren) {
								bool invalidate = false;
								for (Rectangle r : invalidRects) {
									if (r.isInside(activePoint)) {
										invalidate = true;
									}
								}
								if (!invalidate && o->getActiveRectangle().isInside(activePoint)) {
									if (o->hasMouseRightClickUpEvent) {
										o->mouseRightClickUpEvent();
									}
								}
								invalidRects.push_back(o->getActiveRectangle());
							}
						}
					}
				}
				Input::isRButtonDown = false;
				Input::rButtonDown.x = 0;
				Input::rButtonDown.y = 0;
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

shared_ptr<Mouse> Input::getMouse()
{
	return mouse;
}
