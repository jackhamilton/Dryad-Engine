#pragma once
#include <SDL.h>
#include <string>
#include <map>
#include <SDL_ttf.h>

namespace dryad
{

class AssetLibrary
{
public:
	AssetLibrary() : AssetLibrary("res\\") {};
	//relative to build base directory. Starts with a \\ appended.
	AssetLibrary(std::string directoryPath);
	std::string getAsset(std::string nameIncludingExtension);
	//must add subdirectories through the add function first
	std::string getAsset(std::string nameIncludingExtension, std::string subdirectory);
	//name parameter is what you'll fetch it with when retrieving assets
	void addSubdirectory(std::string relativePath, std::string name);
	void openFont(std::string nameWithExtension, int size);
private:
	std::string baseLibraryDirectory;
    std::map<std::string, std::string> subdirectories;
	std::map<std::string, std::pair<TTF_Font*, int>> fonts;
	friend class Text;
	friend class TextField;
	friend class Button;
};

}
