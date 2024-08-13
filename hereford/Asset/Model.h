#pragma once

#include "Mesh.h"

#include <string>
#include <vector>



class Model
{
public:
	std::vector<Texture> mLoadedTextures;
	std::vector<Mesh> mMeshes;
	std::string mDirectory;
	bool mGammaCorrection;

	Model(std::string const& inPath, bool inGamma = false);
	void Draw(unsigned int shaderID);
private:
	void LoadModel(std::string const& path);

	void ProcessNode(struct aiNode* node, const struct aiScene* scene);

	Mesh ProcessMesh(struct aiMesh* mesh, const struct aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(struct aiMaterial* mat, enum aiTextureType type);

	unsigned int SetupTextureFromFile(const std::string& filename, const std::string& directory, bool gamma = false);

};

