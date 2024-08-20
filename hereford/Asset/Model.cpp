#include "Model.h"
#include "Texture.h"
#include "Math/MathAssimpHelper.h"

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <assert.h>

Model::Model(const std::string& inPath)
	: Asset(inPath, EAssetType::MODEL)
{
	Initialize();
}

void Model::Initialize()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(mPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	mDirectory = mPath.substr(0, mPath.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh finalMesh;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		SetVertexBoneDataToDefault(vertex);

		vertex.mPosition = MathAssimpHelper::ConvertAssimpVec3(mesh->mVertices[i]);

		if (mesh->HasNormals())
			vertex.mNormal = MathAssimpHelper::ConvertAssimpVec3(mesh->mNormals[i]);

		if (mesh->mTextureCoords[0])
		{
			// assume only use texcoord at index 0
			vertex.mTexCoords = MathAssimpHelper::ConvertAssimpVec2(mesh->mTextureCoords[0][i]);
			vertex.mTangent = MathAssimpHelper::ConvertAssimpVec3(mesh->mTangents[i]);
			vertex.mBitangent = MathAssimpHelper::ConvertAssimpVec3(mesh->mBitangents[i]);
		}
		else
			vertex.mTexCoords = Vec2(0.0f, 0.0f);

		finalMesh.mVertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			finalMesh.mIndices.push_back(face.mIndices[j]);
	}


	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> tempTextures;

	std::vector<aiTextureType> targetTextureType{ 
		aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS,
		aiTextureType_HEIGHT, aiTextureType_EMISSIVE, aiTextureType_METALNESS,
		aiTextureType_DIFFUSE_ROUGHNESS, aiTextureType_LIGHTMAP
	};

	for (auto tType : targetTextureType)
	{
		tempTextures = LoadMaterialTextures(material, tType, scene);
		finalMesh.mTextures.insert(finalMesh.mTextures.end(), tempTextures.begin(), tempTextures.end());
	}

	ExtractBoneWeightForVertices(finalMesh.mVertices, mesh, scene);
	GenerateGLAsset(finalMesh);

	return finalMesh;
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString filename;
		//TODO: add support for embedded textures
		mat->GetTexture(type, i, &filename);
		bool skip = false;

		std::string path = mDirectory + "/" + filename.C_Str();

		// if current textures already loaded, skip
		//TODO: use dict for better effiency?
		for (unsigned int j = 0; j < mLoadedTextures.size(); j++)
		{
			if (std::strcmp(mLoadedTextures[j].GetFileName().data(), filename.C_Str()) == 0)
			{
				textures.push_back(mLoadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			const aiTexture* embbedTex = scene->GetEmbeddedTexture(filename.C_Str());

			std::string actualFilename = aiScene::GetShortFilename(filename.C_Str());
			std::string path = mDirectory + "/" + actualFilename;
			Texture texture(path.c_str(), embbedTex);
			ETextureType tType;
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				tType = ETextureType::DIFFUSE;
				break;
			case aiTextureType_SPECULAR:
				tType = ETextureType::SPECULAR;
				break;
			case aiTextureType_HEIGHT:
				tType = ETextureType::HEIGHT;
				break;
			case aiTextureType_NORMALS:
				tType = ETextureType::NORMALS;
				break;
			case aiTextureType_EMISSIVE:
				tType = ETextureType::EMISSIVE;
				break;
			case aiTextureType_DIFFUSE_ROUGHNESS:
				tType = ETextureType::DIFFUSE_ROUGHNESS;
				break;
			case aiTextureType_METALNESS:
				tType = ETextureType::METALNESS;
				break;
			case aiTextureType_LIGHTMAP:
				tType = ETextureType::AMBIENT_OCCLUSION;
				break;
			default:
				//TODO: logging
				break;
			}

			texture.SetType(tType);
			textures.push_back(texture);
			mLoadedTextures.push_back(texture);
		}
	}
	return textures;
}



void Model::GenerateGLAsset(Mesh& inMesh)
{
	glGenVertexArrays(1, &inMesh.mVAOID);
	glGenBuffers(1, &inMesh.mVBOID);
	glGenBuffers(1, &inMesh.mEBOID);

	glBindVertexArray(inMesh.mVAOID);
	glBindBuffer(GL_ARRAY_BUFFER, inMesh.mVBOID);
	glBufferData(GL_ARRAY_BUFFER, inMesh.mVertices.size() * sizeof(Vertex), &inMesh.mVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inMesh.mEBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, inMesh.mIndices.size() * sizeof(unsigned int), &inMesh.mIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTexCoords));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mBitangent));
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, mBoneIDs));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mWeights));

	glBindVertexArray(0);
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.mBoneIDs[i] = -1;
		vertex.mWeights[i] = 0.0f;
	}
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		if (vertex.mBoneIDs[i] < 0)
		{
			vertex.mBoneIDs[i] = boneID;
			vertex.mWeights[i] = weight;
			break;
		}
	}
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (mBoneInfoMap.find(boneName) == mBoneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.mID = mBoneCounter;
			newBoneInfo.mOffset = MathAssimpHelper::ConvertAssimpMat4(mesh->mBones[boneIndex]->mOffsetMatrix);
			mBoneInfoMap[boneName] = newBoneInfo;
			boneID = mBoneCounter;
			mBoneCounter++;
		}
		else
		{
			boneID = mBoneInfoMap[boneName].mID;
		}

		assert(boneID != -1);

		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; weightIndex++)
		{
			int vertexID = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexID <= vertices.size());
			SetVertexBoneData(vertices[vertexID], boneID, weight);
		}
		

	}
}
