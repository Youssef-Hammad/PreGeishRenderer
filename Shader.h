#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	// Sets shader active
	void setActive();

	void setInt(std::string name, int value);
	void setFloat(std::string name, float value);
	void setBool(std::string name, bool value);
	void setMat4(std::string name, glm::mat4 value);
	void setVec3(std::string name, glm::vec3 value);

};

#endif // !SHADER_H

