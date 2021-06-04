#pragma once
#include <SDL.h>
#include <memory>
#include "Window.h"
#include "Polygon.h"

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
	bool DrawFilledPolygon(Polygon poly, const SDL_Color color);
	void drawFilledTriangle(Polygon poly, const SDL_Color color);
	void set_pixel(SDL_Surface* surface, int x, int y, Uint32 color);
	Resolution* res;
	SDL_Renderer* renderer;
	friend class World;
	friend class Window;
};
