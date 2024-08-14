#pragma once
#include <string>


enum class EAssetType
{
	RESERVED_DEFAULT = 0,
	MESH,
	TEXTURE,
	MODEL,					// Mesh w/ Textures
	ANIMATION,
	SHADER,
};

class Asset
{
public:
	Asset(const std::string& inPath, EAssetType inType = EAssetType::RESERVED_DEFAULT);
	virtual ~Asset();

	virtual void Initialize() = 0;
	std::string GetPath() const { return mPath; }
	EAssetType GetType() const { return mType; }

protected:
	std::string mPath;
	EAssetType mType;
};

