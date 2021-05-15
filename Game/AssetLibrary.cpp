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

const char* AssetLibrary::getAsset(const char* name, const char* extension)
{
	int memSize = sizeof(char) * (strlen(name) + strlen(extension) + 1 + strlen(baseLibraryDirectory));
	char* strBuilder = (char*)malloc(memSize);
	if (strBuilder) {
		memset(strBuilder, 0, memSize);
		strcpy(strBuilder, name);
		strcat(strBuilder, ".");
		strcat(strBuilder, extension);
		const char* ptr = getAsset(strBuilder);
		free(strBuilder);
		return ptr;
	}
	return nullptr;
}

