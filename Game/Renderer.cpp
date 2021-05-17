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
	if (!renderer) {
		printf("Error: Could not retrieve renderer. One potential cause is a floating scene - make sure all your scenes are part of a world.\n");
	}
	return renderer;
}

void Renderer::render(shared_ptr<SDL_Texture> texture, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture.get(), NULL, &dstrect);
}

void Renderer::render(shared_ptr<SDL_Texture> texture, SDL_Rect srcrect, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture.get(), &srcrect, &dstrect);
}

void Renderer::render(SDL_Texture* texture, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}

void Renderer::render(SDL_Texture* texture, SDL_Rect srcrect, SDL_Rect dstrect)
{
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
}

void Renderer::renderPresent() {
	SDL_RenderPresent(renderer);
}

void Renderer::renderBackground()
{
	SDL_RenderClear(renderer);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(renderer);
}
