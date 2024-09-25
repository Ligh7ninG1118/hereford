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
			 halfWidth, -halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  halfWidth,  0.0f,
			 halfWidth,  halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  halfWidth,  halfWidth,
			 halfWidth,  halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  halfWidth,  halfWidth,
			-halfWidth,  halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  0.0f,  halfWidth,
			-halfWidth, -halfWidth, -halfWidth,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			-halfWidth, -halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 halfWidth, -halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  halfWidth,  0.0f,
			 halfWidth,  halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  halfWidth,  halfWidth,
			 halfWidth,  halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  halfWidth,  halfWidth,
			-halfWidth,  halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  0.0f,  halfWidth,
			-halfWidth, -halfWidth,  halfWidth,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			-halfWidth,  halfWidth,  halfWidth, -1.0f,  0.0f,  0.0f,  halfWidth,  0.0f,
			-halfWidth,  halfWidth, -halfWidth, -1.0f,  0.0f,  0.0f,  halfWidth,  halfWidth,
			-halfWidth, -halfWidth, -halfWidth, -1.0f,  0.0f,  0.0f,  0.0f,  halfWidth,
			-halfWidth, -halfWidth, -halfWidth, -1.0f,  0.0f,  0.0f,  0.0f,  halfWidth,
			-halfWidth, -halfWidth,  halfWidth, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-halfWidth,  halfWidth,  halfWidth, -1.0f,  0.0f,  0.0f,  halfWidth,  0.0f,
			 halfWidth,  halfWidth,  halfWidth,  1.0f,  0.0f,  0.0f,  halfWidth,  0.0f,
			 halfWidth,  halfWidth, -halfWidth,  1.0f,  0.0f,  0.0f,  halfWidth,  halfWidth,
			 halfWidth, -halfWidth, -halfWidth,  1.0f,  0.0f,  0.0f,  0.0f,  halfWidth,
			 halfWidth, -halfWidth, -halfWidth,  1.0f,  0.0f,  0.0f,  0.0f,  halfWidth,
			 halfWidth, -halfWidth,  halfWidth,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 halfWidth,  halfWidth,  halfWidth,  1.0f,  0.0f,  0.0f,  halfWidth,  0.0f,
			-halfWidth, -halfWidth, -halfWidth,  0.0f, -1.0f,  0.0f,  0.0f,  halfWidth,
			 halfWidth, -halfWidth, -halfWidth,  0.0f, -1.0f,  0.0f,  halfWidth,  halfWidth,
			 halfWidth, -halfWidth,  halfWidth,  0.0f, -1.0f,  0.0f,  halfWidth,  0.0f,
			 halfWidth, -halfWidth,  halfWidth,  0.0f, -1.0f,  0.0f,  halfWidth,  0.0f,
			-halfWidth, -halfWidth,  halfWidth,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-halfWidth, -halfWidth, -halfWidth,  0.0f, -1.0f,  0.0f,  0.0f,  halfWidth,
			-halfWidth,  halfWidth, -halfWidth,  0.0f,  1.0f,  0.0f,  0.0f,  halfWidth,
			 halfWidth,  halfWidth, -halfWidth,  0.0f,  1.0f,  0.0f,  halfWidth,  halfWidth,
			 halfWidth,  halfWidth,  halfWidth,  0.0f,  1.0f,  0.0f,  halfWidth,  0.0f,
			 halfWidth,  halfWidth,  halfWidth,  0.0f,  1.0f,  0.0f,  halfWidth,  0.0f,
			-halfWidth,  halfWidth,  halfWidth,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-halfWidth,  halfWidth, -halfWidth,  0.0f,  1.0f,  0.0f,  0.0f,  halfWidth
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
			-halfWidth,		0.0f,		-halfLength,    0.0f, 1.0f, 0.0f,		0.0f, halfLength ,
			halfWidth,		0.0f,		-halfLength,    0.0f, 1.0f, 0.0f,		halfWidth, halfLength ,

			-halfWidth,		0.0f,		halfLength,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f ,
			halfWidth,		0.0f,		-halfLength,    0.0f, 1.0f, 0.0f,		halfWidth, halfLength ,
			halfWidth,		0.0f,		halfLength,		0.0f, 1.0f, 0.0f,		halfWidth, 0.0f };
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
	 glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359f;
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	}*/
private:
	static std::unordered_map<std::string, PrimitiveInfo> mGeneratedPrimitiveMap;
};

