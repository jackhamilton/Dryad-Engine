#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <SDL_ttf.h>

using namespace std;
class AssetLibrary
{
public:
	AssetLibrary() : AssetLibrary("res\\") {};
	//relative to build base directory. Starts with a \\ appended.
	AssetLibrary(string directoryPath);
	string getAsset(string nameIncludingExtension);
	//must add subdirectories through the add function first
	string getAsset(string nameIncludingExtension, string subdirectory);
	//name parameter is what you'll fetch it with when retrieving assets
	void addSubdirectory(string relativePath, string name);
	void openFont(string nameWithExtension, int size);
private:
	string baseLibraryDirectory;
	map<string, string> subdirectories;
	map<string, pair<TTF_Font*, int>> fonts;
	friend class Text;
	friend class TextField;
	friend class Button;
};

