#pragma once
#include <SDL.h>
#include <string.h>

class AssetLibrary
{
public:
	AssetLibrary() : AssetLibrary("res\\") {};
	//relative to build base directory. Starts with a \\ appended.
	AssetLibrary(const char* directoryPath);
	const char* baseLibraryDirectory;
	const char* getAsset(const char* nameIncludingExtension);
	const char* getAsset(const char* name, const char* extension);
};

