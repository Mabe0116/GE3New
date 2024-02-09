#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager();
	}

	return nullptr;
}

void TextureManager::Finalize()
{

}
