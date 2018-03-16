#include <Sprite.h>

Sprite::Sprite(char filename[], Window window)
{
	startedAnimation = false;
	window.sprites.push_back(this);
	SDL_Texture* images[sizeof(filename)];
	Renderer* windowRenderer = window.getRenderer();
	Renderer renderer = *windowRenderer;
	for (int x = 0; x < sizeof(filename); x++) {
		SDL_Surface * tempImage = IMG_Load(&filename[x]);
		images[x] = SDL_CreateTextureFromSurface(
			renderer.getSDLRenderer(), tempImage);
		SDL_FreeSurface(tempImage); 
		//might cause problems? Freeing the surface before rendering
	}
}

pair<int, int> Sprite::getDimensions()
{
	SDL_Texture* source = peekCurrentImage();
	int w, h;
	SDL_QueryTexture(source, NULL, NULL, &w, &h);
	return pair<int, int>(w, h);
}

SDL_Texture * Sprite::getCurrentImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	else {
		if (currentImage != (images.end()--)) {
			currentImage++;
		}
		else {
			currentImage = images.begin();
		}
	}
	return *currentImage;
}

SDL_Texture * Sprite::peekCurrentImage()
{
	if (!startedAnimation) {
		currentImage = images.begin();
		startedAnimation = true;
	}
	return *currentImage;
}

void Sprite::destroy()
{
	while (!images.empty()){
		SDL_DestroyTexture(images.front());
		images.pop_front();
	}
}
