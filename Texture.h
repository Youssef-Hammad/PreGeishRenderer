#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"

struct Material
{
	glm::vec3 ambient; //Ka
	glm::vec3 diffuse; //Kd
	glm::vec3 specular; //Ks
	int illumModel; //1 indicates no specular, 2 indicates specular
	float shininess; //Ns
};

static GLenum current_texture_number = GL_TEXTURE0;

class Texture
{
public:
	unsigned int ID;
	std::string path;
	GLenum texture_number;
	Material material;

	Texture(std::string path, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, int illum, float shine);
	void bind();
	void unbind();
};

#endif // !TEXTURE_H
