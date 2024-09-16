#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(const std::string& inPath)
	:
	Asset(inPath, EAssetType::SHADER)
{
	Initialize();
}

Shader::~Shader()
{
	//printf("Shader Destructor. Path: %s\n", mPath.c_str());
	glDeleteShader(mID);
}


void Shader::Initialize()
{
	auto div = mPath.find_last_of('*');
	std::string vertPath = mPath.substr(0, div);
	std::string fragPath = mPath.substr(div + 1);

	uint32 vertShaderID = GenerateGLAsset(vertPath, GL_VERTEX_SHADER);
	CheckCompileError(vertShaderID, "VERTEX");
	uint32 fragShaderID = GenerateGLAsset(fragPath, GL_FRAGMENT_SHADER);
	CheckCompileError(fragShaderID, "FRAGMENT");

	mID = glCreateProgram();
	glAttachShader(mID, vertShaderID);
	glAttachShader(mID, fragShaderID);
	glLinkProgram(mID);
	CheckCompileError(mID, "PROGRAM");

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

}

uint32 Shader::GenerateGLAsset(const std::string& inPath, GLenum shaderType)
{
	uint32 shaderID;
	std::string shaderCodeStr;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(inPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCodeStr = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		printf("ERROR:SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}

	const char* shaderCode = shaderCodeStr.c_str();


	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	return shaderID;
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