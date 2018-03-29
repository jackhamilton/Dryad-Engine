#pragma once
#include <SDL.h>
#include <Window.h>

class Renderer {
public:
	Renderer(Window* window);
	SDL_Renderer* getSDLRenderer();
	void render(SDL_Texture* texture, SDL_Rect dstrect);
	void render(SDL_Texture * texture, SDL_Rect srcrect, SDL_Rect dstrect);
	void renderBackground();
	void destroy();
private:
	SDL_Renderer* renderer;
};