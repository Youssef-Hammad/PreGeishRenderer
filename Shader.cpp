#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string serializedVertexCode;
	std::string serializedFragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//Ensures the ifstream objects through exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);

		std::stringstream vShaderStream, fShaderStream;

		// Read file's content to a buffer and pass it to streams (not sure if wording is accurate)
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		serializedVertexCode = vShaderStream.str();
		serializedFragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vertexCode = serializedVertexCode.c_str();
	const char* fragmentCode = serializedFragmentCode.c_str();

	unsigned int vertex, fragment;
	int vSuccess;
	int fSuccess;
	char vInfoLog[512];
	char fInfoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &vSuccess);

	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &fSuccess);

	if (!vSuccess)
	{
		glGetShaderInfoLog(vertex, 512, NULL, vInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vInfoLog << std::endl;
	}
	
	if (!fSuccess)
	{
		glGetShaderInfoLog(fragment, 512, NULL, fInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fInfoLog << std::endl;
	}

	int programSuccess;
	char programInfoLog[512];

	ID = glCreateProgram();

	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess)
	{
		glGetProgramInfoLog(ID, 512, NULL, programInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << programInfoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::setActive()
{
	glUseProgram(ID);
}

void Shader::setInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setBool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(std::string name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}