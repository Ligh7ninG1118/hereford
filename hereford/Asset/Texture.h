#pragma once

#include "Asset.h"
#include "Math/Math.h"

enum class ETextureType
{
	RESERVED_DEFAULT = 0,
	DIFFUSE,
	SPECULAR,
	NORMALS,
	HEIGHT,
	EMISSIVE,
	AMBIENT,
	METALLIC,
	ROUGHNESS,
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
	inline Vec2 GetDimension() const { return mDimension; }

private:
	void Initialize(const std::string& inPath, const struct aiTexture* embededdTex = nullptr);
	void GenerateGLAsset(unsigned char* inData, int width, int height, int numComponents, bool gamma = false);

	unsigned int mID;
	ETextureType mType;
	std::string mFileName;

	Vec2 mDimension;
};

