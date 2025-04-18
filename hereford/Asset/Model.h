#pragma once

#include "Math/Math.h"
#include "Asset.h"
#include "Texture.h"
#include <string>
#include <assimp/matrix4x4.h>

#include <vector>
#include <map>

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

struct BoneInfo
{
	int mID;
	Mat4 mOffset;
};

struct Mesh
{
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;

	unsigned int mVAOID;
	unsigned int mVBOID;
	unsigned int mEBOID;
};

class Model : public Asset
{
public:
	std::vector<Mesh> mMeshes;

	Model(const std::string& inPath);
	auto& GetBoneInfoMap() const { return mBoneInfoMap; }
	int& GetBoneCount() { return mBoneCounter; }

	void ManualLoadTexture(Texture tex);

private:
	std::string mDirectory;
	std::vector<class Texture> mLoadedTextures;
	
	void Initialize();

	void ProcessNode(struct aiNode* node, const struct aiScene* scene);
	Mesh ProcessMesh(struct aiMesh* mesh, const struct aiScene* scene);
	std::vector<class Texture> LoadMaterialTextures(struct aiMaterial* mat, enum aiTextureType type, const struct aiScene* scene);
	void GenerateGLAsset(Mesh& inSubMesh);

	std::map<std::string, BoneInfo> mBoneInfoMap;
	int mBoneCounter = 0;

	void SetVertexBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
};
