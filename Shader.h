#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

class Shader
{
public:
	unsigned int ID;

	Shader(std::string vertexPath, std::string fragmentPath);

	void SetActive();

};

#endif // !SHADER_H
