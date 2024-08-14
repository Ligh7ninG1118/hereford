#pragma once
#include "Asset.h"
#include <memory>
#include <unordered_map>

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	template <typename T>
	std::shared_ptr<T> LoadAsset(const std::string& inPath)
	{
		auto it = mAssetsMap.find(inPath);
		if (it != mAssetsMap.end())
		{
			return std::dynamic_pointer_cast<T>(it->second);
		}
		std::shared_ptr<T> asset = std::make_shared<T>(T(inPath));
		mAssetsMap[inPath] = asset;
		return asset;
	}

private:
	std::unordered_map<std::string, std::shared_ptr<Asset>> mAssetsMap;

};

