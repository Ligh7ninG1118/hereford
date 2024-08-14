#pragma once

#include "Math/Math.h"
#include "Asset.h"
#include <string>
#include <vector>

const int MAX_BONE_INFLUENCE = 4;

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

struct Mesh
{
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<class Texture> mTextures;

	unsigned int mVAOID;
	unsigned int mVBOID;
	unsigned int mEBOID;
};

class Model : public Asset
{
public:
	std::vector<Mesh> mMeshes;

	Model(const std::string& inPath);

private:
	std::string mDirectory;
	std::vector<class Texture> mLoadedTextures;
	void Initialize() override;

	void ProcessNode(struct aiNode* node, const struct aiScene* scene);

	Mesh ProcessMesh(struct aiMesh* mesh, const struct aiScene* scene);

	std::vector<class Texture> LoadMaterialTextures(struct aiMaterial* mat, enum aiTextureType type);

	void GenerateGLAsset(Mesh& inSubMesh);
};
