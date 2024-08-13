#include "Math/Math.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>


Model::Model(std::string const& inPath, bool inGamma) 
	: mGammaCorrection(inGamma)
{
	LoadModel(inPath);
}

void Model::Draw(unsigned int shaderID)
{
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		mMeshes[i].Draw(shaderID);
}

void Model::LoadModel(std::string const& inPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(inPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	mDirectory = inPath.substr(0, inPath.find_last_of('/'));
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
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

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

		vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


	return Mesh(vertices, indices, textures);
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
			if (std::strcmp(mLoadedTextures[j].mFilename.data(), filename.C_Str()) == 0)
			{
				textures.push_back(mLoadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.mID = SetupTextureFromFile(filename.C_Str(), this->mDirectory);

			switch (type)
			{
			case aiTextureType_DIFFUSE:
				texture.mType = ETextureType::DIFFUSE;
				break;
			case aiTextureType_SPECULAR:
				texture.mType = ETextureType::SPECULAR;
				break;
			case aiTextureType_HEIGHT:
				texture.mType = ETextureType::HEIGHT;
				break;
			case aiTextureType_NORMALS:
				texture.mType = ETextureType::NORMAL;
				break;
			default:
				//TODO: logging
				break;
			}
			texture.mFilename = filename.C_Str();
			textures.push_back(texture);
			mLoadedTextures.push_back(texture);
		}
	}
	return textures;
}

unsigned int Model::SetupTextureFromFile(const std::string& filename, const std::string& directory, bool gamma)
{

	std::string path = directory + '/' + std::string(filename);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, numComponents;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numComponents, 0);
	if (data)
	{
		GLenum format = 0u;

		switch (numComponents)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			//logging error
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}