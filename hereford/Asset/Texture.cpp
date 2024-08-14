#include "Texture.h"

#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

Texture::Texture(const std::string& inPath)
	: Asset(inPath, EAssetType::TEXTURE)
{
	Initialize();
}

Texture::~Texture()
{
}

void Texture::Initialize()
{
	GenerateGLAsset();
}

void Texture::GenerateGLAsset(bool gamma)
{
	glGenTextures(1, &mID);

	int width, height, numComponents;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(mPath.c_str(), &width, &height, &numComponents, 0);
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

		glBindTexture(GL_TEXTURE_2D, mID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		mFileName = mPath.substr(mPath.find_last_of("/"));
	}
	else
	{
		std::cout << "Texture failed to load at path: " << mPath << std::endl;
		stbi_image_free(data);
	}
}
