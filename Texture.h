#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <glm/glm/glm.hpp>
#include <glad/glad.h>
#include "stb_image.h"

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Texture
{
private:
	static GLenum current_texture_number;
public:
	unsigned int ID;
	std::string path;
	GLenum texture_number;

	Material material;

	Texture(std::string path,glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shine);
	void bind();
	void unbind();
};

#endif // !TEXTURE_H