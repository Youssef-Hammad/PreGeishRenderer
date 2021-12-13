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

class Texture
{
public:
	unsigned int ID;
	std::string texPath;
	GLenum texture_number;
	Material material;

	Texture(GLenum TextureNumber, std::string path, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, int illum, float shine);
	Texture(GLenum TextureNumber, std::string path);
	void bind();
	void unbind();
};

#endif // !TEXTURE_H
