#include "Model.h"
#include "Texture.h"
#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

Model::Model(const std::string& inPath)
	: Asset(inPath, EAssetType::MODEL)
{
	Initialize();
}

void Model::Initialize()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(mPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

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
	Mesh finalSubMesh;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.mPosition = Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasNormals())
			vertex.mNormal = Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);


		if (mesh->mTextureCoords[0])
		{
			// assume only use texcoord at index 0
			vertex.mTexCoords = Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vertex.mTangent = Vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.mBitangent = Vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else
			vertex.mTexCoords = Vec2(0.0f, 0.0f);

		finalSubMesh.mVertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			finalSubMesh.mIndices.push_back(face.mIndices[j]);
	}

	GenerateGLAsset(finalSubMesh);

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> tempTextures;

	std::vector<aiTextureType> targetTextureType{ 
		aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS,
		aiTextureType_HEIGHT, aiTextureType_EMISSIVE, aiTextureType_METALNESS,
		aiTextureType_DIFFUSE_ROUGHNESS, aiTextureType_LIGHTMAP
	};

	for (auto tType : targetTextureType)
	{
		tempTextures = LoadMaterialTextures(material, tType);
		finalSubMesh.mTextures.insert(finalSubMesh.mTextures.end(), tempTextures.begin(), tempTextures.end());
	}

	return finalSubMesh;
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString filename;
		mat->GetTexture(type, i, &filename);
		bool skip = false;

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
			Texture texture(mDirectory + "/" + filename.C_Str());
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