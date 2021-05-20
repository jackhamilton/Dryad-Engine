#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <list>
#include <vector>
#include <memory>
#include <string>
#include "Size.h"
#include "Point.h"
#include "ModifiableProperty.h"

class Renderer;

using namespace std;
class Sprite {
public:
	//Blank constructor
	Sprite();
	//Initialize using list of textures
	Sprite(vector<SDL_Surface*> images) : Sprite(images, 1, false) {};
	Sprite(vector<SDL_Surface*> images, int fps, bool loop);
	//Initialize using image files
    Sprite(vector<string> filenames) : Sprite(filenames, 1, false) {};
    Sprite(vector<string> filenames, int fps, bool loop);
	~Sprite();

	list<SDL_Texture*> images;
	double renderTimeBuffer;
	bool loaded;

    virtual pair<int, int> getDimensions();
	//Gets current image, advances
	virtual SDL_Texture* getCurrentImage();
	//Does not advance, but starts animation if not started
	virtual SDL_Texture* peekCurrentImage();
	virtual void nextImage();
	virtual void render(shared_ptr<Renderer> renderer, Point locationMod);
	virtual void loadTextures(shared_ptr<Renderer> renderer);
	void tick();

	vector<string> getFilenames() {
		return filenames;
	}
	void setLocation(Point location) {
		Sprite::location = location;
	}
	Point getLocation() {
		return location;
	}
	int getFPS() {
		return fps;
	}
	void setAnimationSpeed(double speed) {
		animationSpeed.clearModifiers();
		animationSpeed.addModifier(speed);
	}
	void setPaused(bool pause) {
		Sprite::paused = pause;
	}
	bool isOnFinalFrame();
protected:
	void initDefaultParams(int fps);
	std::list<SDL_Texture*>::iterator currentImage;
	bool paused;
	Point location;
	Size size;
	int fps;
private:
	bool startedAnimation;
	bool loopAnimation;
	ModifiableProperty<double, double> animationSpeed;
	double animationSpeedMultiplier;
	//True if sprite uses files, false if sprite uses a textureset
	bool fileBased;
	std::vector<string> filenames;
	std::vector<SDL_Surface*> surfaces;
	friend class GameObject;
};
