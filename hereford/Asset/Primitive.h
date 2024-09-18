#pragma once
#include "Math/Math.h"
#include <unordered_map>
#include <glad/glad.h>
#include <string>

//#include 

struct PrimitiveInfo
{
	uint32 mVAO;
	uint32 mVBO;
};


class Primitive
{
public:
	static void GenerateCube(PrimitiveInfo& outInfo, float width = 1.0f)
	{
		std::string lookupStr = "cube" + std::to_string(width);
		if (mGeneratedPrimitiveMap.find(lookupStr) != mGeneratedPrimitiveMap.end())
		{
			PrimitiveInfo savedInfo = mGeneratedPrimitiveMap[lookupStr];
			outInfo.mVAO = savedInfo.mVAO;
			outInfo.mVBO = savedInfo.mVBO;
			return;
		}

		float halfWidth = width * 0.5f;

		float vertices[] = {
			// positions          // normals           // texture coords
			-halfWidth, -halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 halfWidth, -halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 halfWidth,  halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 halfWidth,  halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-halfWidth,  halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-halfWidth, -halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			-halfWidth, -halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 halfWidth, -halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 halfWidth,  halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 halfWidth,  halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-halfWidth,  halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-halfWidth, -halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			-halfWidth,  halfWidth,  halfWidth, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-halfWidth,  halfWidth, -halfWidth, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-halfWidth, -halfWidth, -halfWidth, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-halfWidth, -halfWidth, -halfWidth, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-halfWidth, -halfWidth,  halfWidth, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-halfWidth,  halfWidth,  halfWidth, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 halfWidth,  halfWidth,  halfWidth,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 halfWidth,  halfWidth, -halfWidth,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 halfWidth, -halfWidth, -halfWidth,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 halfWidth, -halfWidth, -halfWidth,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 halfWidth, -halfWidth,  halfWidth,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 halfWidth,  halfWidth,  halfWidth,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-halfWidth, -halfWidth, -halfWidth,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 halfWidth, -halfWidth, -halfWidth,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 halfWidth, -halfWidth,  halfWidth,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 halfWidth, -halfWidth,  halfWidth,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-halfWidth, -halfWidth,  halfWidth,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-halfWidth, -halfWidth, -halfWidth,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			-halfWidth,  halfWidth, -halfWidth,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 halfWidth,  halfWidth, -halfWidth,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 halfWidth,  halfWidth,  halfWidth,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 halfWidth,  halfWidth,  halfWidth,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-halfWidth,  halfWidth,  halfWidth,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-halfWidth,  halfWidth, -halfWidth,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};
		glGenVertexArrays(1, &outInfo.mVAO);
		glGenBuffers(1, &outInfo.mVBO);
		glBindVertexArray(outInfo.mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, outInfo.mVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Cache generated cube for future use
		PrimitiveInfo savedInfo{ outInfo.mVAO, outInfo.mVBO };
		mGeneratedPrimitiveMap[lookupStr] = savedInfo;
	}

	static void GeneratePlane(PrimitiveInfo& outInfo, float width = 10.0f, float length = 10.0f)
	{
		std::string lookupStr = "plane" + std::to_string(width) + std::to_string(length);
		if (mGeneratedPrimitiveMap.find(lookupStr) != mGeneratedPrimitiveMap.end())
		{
			PrimitiveInfo savedInfo = mGeneratedPrimitiveMap[lookupStr];
			outInfo.mVAO = savedInfo.mVAO;
			outInfo.mVBO = savedInfo.mVBO;
			return;
		}

		float halfWidth = width * 0.5f;
		float halfLength = length * 0.5f;

		float vertices[] = {
			// positions								// normals				// texture coords
			-halfWidth,		0.0f,		halfLength,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f ,
			-halfWidth,		0.0f,		-halfLength,    0.0f, 1.0f, 0.0f,		0.0f, 1.0f ,
			halfWidth,		0.0f,		-halfLength,    0.0f, 1.0f, 0.0f,		1.0f, 1.0f ,

			-halfWidth,		0.0f,		halfLength,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f ,
			halfWidth,		0.0f,		-halfLength,    0.0f, 1.0f, 0.0f,		1.0f, 1.0f ,
			halfWidth,		0.0f,		halfLength,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f };
		glGenVertexArrays(1, &outInfo.mVAO);
		glGenBuffers(1, &outInfo.mVBO);
		glBindVertexArray(outInfo.mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, outInfo.mVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Cache generated cube for future use
		PrimitiveInfo savedInfo{ outInfo.mVAO, outInfo.mVBO };
		mGeneratedPrimitiveMap[lookupStr] = savedInfo;
	}

	/*static void GenerateSphere(PrimitiveInfo& outInfo, int subDivision = 4, float radius = 1.0f)
	{

	}*/
private:
	static std::unordered_map<std::string, PrimitiveInfo> mGeneratedPrimitiveMap;
};

