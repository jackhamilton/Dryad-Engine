#include "Renderer.h"

Renderer::Renderer(SDL_Window* window)
{
	renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        printf("%s", SDL_GetError());
    }
}

SDL_Renderer* Renderer::getSDLRenderer()
{
	return renderer;
}

void Renderer::render(SDL_Texture* texture, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_RenderPresent(renderer);
}

void Renderer::render(SDL_Texture* texture, SDL_Rect srcrect, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
	SDL_RenderPresent(renderer);
}

void Renderer::renderBackground()
{
	SDL_RenderClear(renderer);
}

void Renderer::destroy()
{
	SDL_DestroyRenderer(renderer);
}
