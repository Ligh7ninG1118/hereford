#pragma once

#include "Math/Math.h"
#include "Asset.h"

#include <string>

#include <glad/glad.h>

class Shader : public Asset
{
public:
	Shader(const std::string& inPath);

	inline void Use() { glUseProgram(mID); };
	inline uint32 GetID() const { return mID; }

private:
	void Initialize();
	uint32 GenerateGLAsset(const std::string& inPath, GLenum shaderType);
	void CheckCompileError(GLuint shader, std::string type);

	// TODO: A Uniform map of (name, value) for shader instance?
	uint32 mID;
};