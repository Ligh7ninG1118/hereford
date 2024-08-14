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
	inline Uint32 GetID() const { return mID; }

private:
	void Initialize() override;
	Uint32 GenerateGLAsset(const std::string& inPath, GLenum shaderType);
	void CheckCompileError(GLuint shader, std::string type);

	Uint32 mID;
};