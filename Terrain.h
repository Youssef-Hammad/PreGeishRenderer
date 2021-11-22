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
	const int VERTEX_COUNT = 128;

	
	//RenderableObject* terrainModel;
	
	void loadVAO();
	

public:
	unsigned int VAO,VBO;
	float x;
	float z;
	int verticesSize;
	float* vertices;
	Texture* texture;
	Shader* shaderProgram;
	Terrain(int gridX, int gridZ, std::string _texturePath, Shader* _shaderProgram);
	void generateTerrain();
	void draw();

	~Terrain();
};

#endif // !TERRAIN_H
