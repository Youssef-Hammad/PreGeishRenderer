#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <glm/glm/glm.hpp>
#include <glad/glad.h>

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class Texture
{
public:
	unsigned int ID;
	std::string path;

	Material material;

	Texture();
	void bind();
	void unbind();

	~Texture();
};

#endif // !TEXTURE_H