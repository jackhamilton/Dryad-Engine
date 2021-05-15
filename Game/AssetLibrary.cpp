#include "AssetLibrary.h"

AssetLibrary::AssetLibrary(const char* directoryPath){
	baseLibraryDirectory = strcat(SDL_GetBasePath(), directoryPath);
}

const char* AssetLibrary::getAsset(const char* nameIncludingExtension)
{
	int memSize = sizeof(char) * (strlen(nameIncludingExtension) + strlen(baseLibraryDirectory));
	char* strBuilder = (char*)malloc(memSize);
	if (strBuilder) {
		memset(strBuilder, 0, memSize);
		strcat(strBuilder, baseLibraryDirectory);
		strcat(strBuilder, nameIncludingExtension);
	}
	return strBuilder;
}

const char* AssetLibrary::getAsset(const char* nameIncludingExtension, const char* subdirectory)
{
	if (!subdirectories[subdirectory]) {
		return nullptr;
	}
	int memSize = sizeof(char) * (strlen(nameIncludingExtension) + strlen(baseLibraryDirectory) + strlen(subdirectories[subdirectory]));
	char* strBuilder = (char*)malloc(memSize);
	if (strBuilder) {
		memset(strBuilder, 0, memSize);
		strcat(strBuilder, baseLibraryDirectory);
		strcat(strBuilder, subdirectories[subdirectory]);
		strcat(strBuilder, nameIncludingExtension);
	}
	return strBuilder;
}

void AssetLibrary::addSubdirectory(const char* relativePath, const char* name)
{
	//might potentially need to strcopy the relativePath to avoid loss of reference?
	subdirectories[name] = relativePath;
}


