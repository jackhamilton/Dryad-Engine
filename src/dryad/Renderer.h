#pragma once
#include <SDL.h>
#include <memory>
#include "Window.h"
#include "Polygon.h"
#include <vector>
#include "Light.h"

namespace dryad
{

class Renderer {
public:
	Renderer(SDL_Window* window);
	~Renderer();
	SDL_Renderer* getSDLRenderer();
	void render(std::shared_ptr<SDL_Texture> texture, SDL_Rect dstrect);
	void render(std::shared_ptr<SDL_Texture> texture, SDL_Rect srcrect, SDL_Rect dstrect);
	void render(SDL_Texture* texture, SDL_Rect dstrect);
	void render(SDL_Texture* texture, SDL_Rect srcrect, SDL_Rect dstrect);
	void renderPresent();
	void renderBackground();
	void computeLighting(double globalIllumination, std::pair<std::vector<Light>, std::vector<std::vector<Polygon>>> masks);
private:
	bool DrawFilledPolygon(Polygon poly, const SDL_Color color);
	void renderLine(LineData a, const SDL_Color color);
	void drawFilledTriangle(SDL_Surface* surf, Polygon poly, const SDL_Color color);
	void drawFilledTriangle(Polygon poly, const SDL_Color color);
	void set_pixel(SDL_Surface* surface, int x, int y, Uint32 color);
	Resolution* res;
	SDL_Renderer* renderer;
	friend class World;
	friend class Scene;
	friend class Window;
};

}
