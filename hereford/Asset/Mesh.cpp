#include "Mesh.h"
#include <glad/glad.h>


Mesh::Mesh(std::vector<Vertex> inVertices, std::vector<unsigned int> inIndices, std::vector<Texture> inTextures)
	:
	mVertices(inVertices),
	mIndices(inIndices),
	mTextures(inTextures)
{
	SetupMesh();
}


//TODO: Move this logic to RenderComponent, use flags to indicate render mode
void Mesh::Draw(unsigned int shaderID)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < mTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string texStr;
		switch (mTextures[i].mType)
		{
		case ETextureType::DIFFUSE:
			//TODO: Check if string cantanation is correct
			texStr = "tex_diffuse_" + diffuseNr++;
			break;
		case ETextureType::SPECULAR:
			texStr = "tex_specular_" + specularNr++;
			break;
		case ETextureType::NORMAL:
			texStr = "tex_normal_" + normalNr++;
			break;
		case ETextureType::HEIGHT:
			texStr = "tex_height_" + heightNr++;
			break;
		default:
			break;
		}

		glUniform1i(glGetUniformLocation(shaderID, texStr.c_str()), i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i].mID);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mIndices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

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