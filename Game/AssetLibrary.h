#pragma once
#include <SDL.h>
#include <string.h>
#include <map>

class AssetLibrary
{
public:
	AssetLibrary() : AssetLibrary("res\\") {};
	//relative to build base directory. Starts with a \\ appended.
	AssetLibrary(const char* directoryPath);
	const char* getAsset(const char* nameIncludingExtension);
	//must add subdirectories through the add function first
	const char* getAsset(const char* nameIncludingExtension, const char* subdirectory);
	//name parameter is what you'll fetch it with when retrieving assets
	void addSubdirectory(const char* relativePath, const char* name);
private:
	const char* baseLibraryDirectory;
	std::map<const char*, const char*> subdirectories;
};

