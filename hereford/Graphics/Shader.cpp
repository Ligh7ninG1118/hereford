#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		printf("ERROR:SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;


	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileError(ID, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileError(ID, "FRAGMENT");


	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	CheckCompileError(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const unsigned int& shaderID, const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}

void Shader::SetInt(const unsigned int& shaderID, const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}
void Shader::SetFloat(const unsigned int& shaderID, const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::SetVec2(const unsigned int& shaderID, const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec2(const unsigned int& shaderID, const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(shaderID, name.c_str()), x, y);
}
void Shader::SetVec3(const unsigned int& shaderID, const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec3(const unsigned int& shaderID, const std::string& name, const Vector3& value)
{
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), value.mX, value.mY, value.mZ);
}
void Shader::SetVec3(const unsigned int& shaderID, const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), x, y, z);
}
void Shader::SetVec4(const unsigned int& shaderID, const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec4(const unsigned int& shaderID, const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(shaderID, name.c_str()), x, y, z, w);
}
void Shader::SetMat2(const unsigned int& shaderID, const std::string& name, const glm::mat2& mat)
{
	glUniformMatrix2fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::SetMat3(const unsigned int& shaderID, const std::string& name, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::SetMat4(const unsigned int& shaderID, const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::CheckCompileError(GLuint shader, std::string type)
{
	int success;
	char infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n", type, infoLog);
			
		};
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n", type, infoLog);
		};
	}
}