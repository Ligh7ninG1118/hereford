#pragma once

#include "Asset.h"

enum class ETextureType
{
	DIFFUSE = 0,
	SPECULAR,
	NORMALS,
	HEIGHT,
	EMISSIVE,
	AMBIENT,
	METALNESS,
	DIFFUSE_ROUGHNESS,
	AMBIENT_OCCLUSION,

};

class Texture : public Asset
{
public:
	Texture(const std::string& inPath);
	~Texture();

	inline unsigned int GetID() const { return mID; }
	inline ETextureType GetType() const { return mType; }
	inline void SetType(ETextureType inType) { mType = inType; }
	inline std::string GetFileName() const { return mFileName; }

private:
	void Initialize() override;
	void GenerateGLAsset(bool gamma = false);

	unsigned int mID;
	ETextureType mType;
	std::string mFileName;
};

