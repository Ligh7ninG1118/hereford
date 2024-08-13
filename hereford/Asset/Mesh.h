#pragma once

#include "Math/Math.h"
#include <string>
#include <vector>

const int MAX_BONE_INFLUENCE = 4;

enum class ETextureType
{
	DIFFUSE = 0,
	SPECULAR,
	NORMAL,
	HEIGHT,
};


struct Vertex
{
	Vec3 mPosition;
	Vec3 mNormal;
	Vec2 mTexCoords;
	Vec3 mTangent;
	Vec3 mBitangent;

	int mBoneIDs[MAX_BONE_INFLUENCE];
	float mWeights[MAX_BONE_INFLUENCE];
};

struct Texture
{
	unsigned int mID;
	ETextureType mType;
	std::string mFilename;
};

class Mesh
{
public:
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	Mesh(std::vector<Vertex> inVertices, std::vector<unsigned int> inIndices, std::vector<Texture> inTextures);
	void Draw(unsigned int shaderID);
private:
	
	void SetupMesh();
};
