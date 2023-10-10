#pragma once
#include <iostream>
#include <glm/glm/gtc/matrix_transform.hpp>

class Shader;

class Water
{
	const float size = 50;
	const std::string vShader = "shaders/vertex\ shaders/WaterVertexShader.vert";
	const std::string fShader = "shaders/fragment\ shaders/WaterFragmentShader.frag";
	glm::vec3 Position;
public:
	unsigned int VAO, VBO;
	float vertices[18] = { 
		-size,1,-size,
		-size,1,size,
		size,1,-size,
		size,1,size,
		size,1,-size,
		-size,1,size
	};
	Shader* shader;

	Water(glm::vec3 position);
	~Water();
	void draw();
};