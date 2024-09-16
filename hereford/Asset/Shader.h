#pragma once

#include "Math/Math.h"
#include "Asset.h"

#include <string>

#include <glad/glad.h>

class Shader : public Asset
{
public:
	Shader(const std::string& inPath);
	~Shader();

	inline void Use() { glUseProgram(mID); };
	inline uint32 GetID() const { return mID; }

private:
	void Initialize();
	uint32 GenerateGLAsset(const std::string& inPath, GLenum shaderType);
	void CheckCompileError(GLuint shader, std::string type);

	// TODO: A Uniform map of (name, value) for shader instance?
	uint32 mID;

	// Shader Operations
public:
	inline void SetBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(mID, name.c_str()), (int)value);
	}
	inline void SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
	}
	inline void SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
	}
	inline void SetVec2(const std::string& name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(mID, name.c_str()), x, y);
	}
	inline void SetVec2(const std::string& name, const Vector2& value)
	{
		glUniform2f(glGetUniformLocation(mID, name.c_str()), value.mX, value.mY);
	}
	inline void SetVec3(const std::string& name, const Vector3& value)
	{
		glUniform3f(glGetUniformLocation(mID, name.c_str()), value.mX, value.mY, value.mZ);
	}
	inline void SetVec3(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(mID, name.c_str()), x, y, z);
	}
	inline void SetVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(mID, name.c_str()), x, y, z, w);
	}
	inline void SetMat4(const std::string& name, const Matrix4x4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &mat.m[0][0]);
	}
};