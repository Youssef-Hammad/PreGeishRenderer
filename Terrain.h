#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include "Texture.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <iostream>

class Terrain
{
private:
	const float SIZE = 800;
	const float MAX_HEIGHT = 40;
	const float MAX_PIXEL_COLOUR = 256 * 256 * 256;

	float getHeight(int x,int y, int imgWidth, int imgHeight, unsigned char* image);
	float getRGB(int x, int y, unsigned char* image, int imgHeight);
	glm::vec3 calculateNormal(int x, int z, int imgWidth, int imgHeight, unsigned char* image);
	

public:
	unsigned int VAO,VBO;
	float x;
	float z;
	int verticesSize;
	float* vertices;
	Texture* texture;
	Shader* shaderProgram;
	Terrain(int gridX, int gridZ, std::string _texturePath, Shader* _shaderProgram, std::string heightMapPath);
	void generateTerrain(std::string heightMapPath);
	void draw();

	~Terrain();
};

#endif // !TERRAIN_H
