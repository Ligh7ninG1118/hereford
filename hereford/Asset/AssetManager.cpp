#include "AssetManager.h"


AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	//TODO: need to manually clear all pointers?
	mAssetsMap.clear();
}
