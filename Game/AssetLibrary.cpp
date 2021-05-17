#include "AssetLibrary.h"

AssetLibrary::AssetLibrary(string directoryPath){
	baseLibraryDirectory = string(SDL_GetBasePath()) + directoryPath;
}

string AssetLibrary::getAsset(string nameIncludingExtension)
{
	return baseLibraryDirectory + nameIncludingExtension;
}

string AssetLibrary::getAsset(string nameIncludingExtension, string subdirectory)
{
	if (subdirectories[subdirectory] == "") {
		return "";
	}
	return baseLibraryDirectory + subdirectories[subdirectory] + nameIncludingExtension;
}

void AssetLibrary::addSubdirectory(string relativePath, string name)
{
	//might potentially need to strcopy the relativePath to avoid loss of reference?
	subdirectories[name] = relativePath + "\\";
}

void AssetLibrary::openFont(string name, int size)
{
	string fontName = baseLibraryDirectory + "fonts\\" + name + ".ttf";
	if (!TTF_WasInit()) {
		TTF_Init();
	}
	TTF_Font* fontTTF;
	//TODO: I do not properly close these
	if (!(fontTTF = TTF_OpenFont(fontName.c_str(), size))) { //this opens a font style and sets a size
		string text = TTF_GetError();
		printf(text.c_str());
	}
	fonts[name] = make_pair(fontTTF, size);
}