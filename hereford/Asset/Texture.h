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
	Texture(const std::string& inPath, const struct aiTexture* embededdTex = nullptr);
	~Texture();

	inline unsigned int GetID() const { return mID; }
	inline ETextureType GetType() const { return mType; }
	inline void SetType(ETextureType inType) { mType = inType; }
	inline std::string GetFileName() const { return mFileName; }

private:
	void Initialize(const std::string& inPath, const struct aiTexture* embededdTex = nullptr);
	void GenerateGLAsset(unsigned char* inData, int width, int height, int numComponents, bool gamma = false);

	unsigned int mID;
	ETextureType mType;
	std::string mFileName;
};

