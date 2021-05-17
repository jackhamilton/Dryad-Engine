#pragma once
#include <SDL.h>
#include <memory>
#include "Window.h"

class Renderer {
public:
	Renderer(SDL_Window* window);
	~Renderer();
	SDL_Renderer* getSDLRenderer();
	void render(shared_ptr<SDL_Texture> texture, SDL_Rect dstrect);
	void render(shared_ptr<SDL_Texture> texture, SDL_Rect srcrect, SDL_Rect dstrect);
	void render(SDL_Texture* texture, SDL_Rect dstrect);
	void render(SDL_Texture* texture, SDL_Rect srcrect, SDL_Rect dstrect);
	void renderPresent();
	void renderBackground();
private:
	SDL_Renderer* renderer;
};
