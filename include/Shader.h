#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;

	Shader(std::string vertexPath, std::string fragmentPath);

	void SetActive(); // Binds shader program to OpenGL

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 mat);
	void setVec3(const std::string& name, glm::vec3 vec);

};

#endif // !SHADER_H
