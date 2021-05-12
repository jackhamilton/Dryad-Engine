#pragma once
#include "Sprite.h"
#include "Renderer.h"
using namespace std;

class CompositeSprite :
	public Sprite
{
public:
	//Requirements: all added lists must have the same framecount.
	CompositeSprite() : CompositeSprite(20) {};
	CompositeSprite(int fps);
	//Can only be called before added to the world
	//Adds a new layer with the given vector as the set of image states
	//Files will always be on top of surfaces. TODO: fix this, and 
	//add logic to sync the framecounts of the two types in the add methods
	//First added surface must also be larger than all sequential surfaces. TODO: fix this (blit onto generic bg).
	void addSpriteFromSurfaces(vector<SDL_Surface*> surfaces, Point offset);
	void addSpriteFromSurfaces(vector<SDL_Surface*> surfaces);
	void addSpriteFromFiles(vector<char*> files, Point offset);
	void addSpriteFromFiles(vector<char*> files);
	void loadTextures(Renderer* renderer);
	void setCenterAll(bool centered);
	~CompositeSprite();
private:
	list<vector<char*>> imageFileStack;
	list<vector<SDL_Surface*>> imageSurfaceStack;
	vector<Point> fileOffsets;
	vector<Point> surfaceOffsets;
	bool centerAll;
};

