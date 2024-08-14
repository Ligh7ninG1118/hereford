#pragma once
#include <string>
#include <glad/glad.h>
#include <Math/Math.h>

class ShaderOp
{
public:
	inline static void SetBool(const unsigned int& shaderID, const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
	}
	inline static void SetInt(const unsigned int& shaderID, const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
	}
	inline static void SetFloat(const unsigned int& shaderID, const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
	}
	inline static void SetVec2(const unsigned int& shaderID, const std::string& name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(shaderID, name.c_str()), x, y);
	}
	inline static void SetVec3(const unsigned int& shaderID, const std::string& name, const Vector3& value)
	{
		glUniform3f(glGetUniformLocation(shaderID, name.c_str()), value.mX, value.mY, value.mZ);
	}
	inline static void SetVec3(const unsigned int& shaderID, const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(shaderID, name.c_str()), x, y, z);
	}
	inline static void SetVec4(const unsigned int& shaderID, const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(shaderID, name.c_str()), x, y, z, w);
	}
	inline static void SetMat4(const unsigned int& shaderID, const std::string& name, const Matrix4x4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat.m[0][0]);
	}
};

