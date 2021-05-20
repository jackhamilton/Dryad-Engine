#pragma once
#include <memory>
#include <string>
#include "Sprite.h"
#include "Renderer.h"
using namespace std;

class CompositeSprite :
	public Sprite
{
public:
	//Requirements: all added lists must have the same framecount.
	CompositeSprite() : CompositeSprite(1) {};
	CompositeSprite(int fps);
	//Can only be called before added to the world
	//Adds a new layer with the given vector as the set of image states
	//Files will always be on top of surfaces. TODO: fix this, and 
	//add logic to sync the framecounts of the two types in the add methods
	//First added surface must also be larger than all sequential surfaces. TODO: fix this (blit onto generic bg).
	void addSpriteFromSurfaces(vector<SDL_Surface*> surfaces, Point offset, int clip);
	void addSpriteFromSurfaces(vector<SDL_Surface*> surfaces);
	void addSpriteFromFiles(vector<string> files, Point offset, int clip);
	void addSpriteFromFiles(vector<string> files);
	void loadTextures(shared_ptr<Renderer> renderer);
	//Unloads top texture, loads any in stacks
	void reloadTopTextureFromStack();
	void reloadAllTextures();
	void setCenterAll(bool centered);
private:
	list<vector<string>> imageFileStack;
	list<vector<SDL_Surface*>> imageSurfaceStack;
	//first is offset, second is clip
	vector<pair<Point, int>> fileOffsets;
	vector<pair<Point, int>> surfaceOffsets;
	bool centerAll;
};

