#include "AssetLibrary.h"

namespace dryad
{

AssetLibrary::AssetLibrary(std::string directoryPath){
	baseLibraryDirectory = std::string(SDL_GetBasePath()) + directoryPath;
}

std::string AssetLibrary::getAsset(std::string nameIncludingExtension)
{
	return baseLibraryDirectory + nameIncludingExtension;
}

std::string AssetLibrary::getAsset(std::string nameIncludingExtension, std::string subdirectory)
{
	if (subdirectories[subdirectory] == "") {
		return "";
	}
	return baseLibraryDirectory + subdirectories[subdirectory] + nameIncludingExtension;
}

void AssetLibrary::addSubdirectory(std::string relativePath, std::string name)
{
	//might potentially need to strcopy the relativePath to avoid loss of reference?
	subdirectories[name] = relativePath + "\\";
}

void AssetLibrary::openFont(std::string name, int size)
{
	std::string fontName = baseLibraryDirectory + "fonts\\" + name + ".ttf";
	if (!TTF_WasInit()) {
		TTF_Init();
	}
	TTF_Font* fontTTF;
	//TODO: I do not properly close these
	if (!(fontTTF = TTF_OpenFont(fontName.c_str(), size))) { //this opens a font style and sets a size
		std::string text = TTF_GetError();
		printf(text.c_str());
	}
	fonts[name] = std::make_pair(fontTTF, size);
}

}
