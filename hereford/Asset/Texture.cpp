#include "Texture.h"

#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

Texture::Texture(const std::string& inPath, const aiTexture* embededdTex)
	: Asset(inPath, EAssetType::TEXTURE)
{
	Initialize(inPath, embededdTex);
}

Texture::~Texture()
{
}

void Texture::Initialize(const std::string& inPath, const aiTexture* embededdTex)
{
	unsigned char* data;
	int width, height, numComponents;

	if (embededdTex)
	{
		auto aiData = embededdTex->pcData;
		int bufferSize = embededdTex->mWidth;

		data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(aiData), bufferSize, &width, &height, &numComponents, 0);
	}
	else
	{
		//stbi_set_flip_vertically_on_load(true);
		data = stbi_load(mPath.c_str(), &width, &height, &numComponents, 0);
	}


	if(data)
	{
		GenerateGLAsset(data, width, height, numComponents);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << mPath << std::endl;
		stbi_image_free(data);
	}
}

void Texture::GenerateGLAsset(unsigned char* inData, int width, int height, int numComponents, bool gamma)
{
	glGenTextures(1, &mID);

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

	glBindTexture(GL_TEXTURE_2D, mID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, inData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(reinterpret_cast<void*>(inData));

	mFileName = mPath.substr(mPath.find_last_of("/")+1);
}
