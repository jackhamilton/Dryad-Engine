#pragma once
#include <memory>
#include <string>
#include "Sprite.h"
#include "Renderer.h"

namespace dryad
{

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
	void addSpriteFromSurfaces(std::vector<SDL_Surface*> surfaces, Point offset, int clip);
	void addSpriteFromSurfaces(std::vector<SDL_Surface*> surfaces);
	void addSpriteFromFiles(std::vector<std::string> files, Point offset, int clip);
	void addSpriteFromFiles(std::vector<std::string> files);
	void loadTextures(std::shared_ptr<Renderer> renderer);
	//Unloads top texture, loads any in stacks
	void reloadTopTextureFromStack();
	void reloadAllTextures();
	void setCenterAll(bool centered);
private:
	std::list<std::vector<std::string>> imageFileStack;
	std::list<std::vector<SDL_Surface*>> imageSurfaceStack;
	//first is offset, second is clip
	std::vector<std::pair<Point, int>> fileOffsets;
	std::vector<std::pair<Point, int>> surfaceOffsets;
	bool centerAll;
};


}
