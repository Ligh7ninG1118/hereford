#pragma once

#include "../Math/Math.h"

#include <string>

#include <glad/glad.h>

class Shader
{
public:
	unsigned int ID;
	Shader() {};
	Shader(const char* vertexPath, const char* fragmentPath);

	void Use();

	static void SetBool(const unsigned int& shaderID, const std::string& name, bool value);
	static void SetInt(const unsigned int& shaderID, const std::string& name, int value);
	static void SetFloat(const unsigned int& shaderID, const std::string& name, float value);

	static void SetVec2(const unsigned int& shaderID, const std::string& name, float x, float y);
	static void SetVec3(const unsigned int& shaderID, const std::string& name, const Vector3& value);
	static void SetVec3(const unsigned int& shaderID, const std::string& name, float x, float y, float z);
	static void SetVec4(const unsigned int& shaderID, const std::string& name, float x, float y, float z, float w);
	static void SetMat4(const unsigned int& shaderID, const std::string& name, const Matrix4x4& mat);


private:
	void CheckCompileError(GLuint shader, std::string type);
};