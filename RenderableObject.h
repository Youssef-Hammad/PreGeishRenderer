#pragma once
#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "fast_obj.h"
#include "Shader.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include <iostream>

class RenderableObject
{
private:
	void load_vertices();
public:
	fastObjMesh* mesh;
	float* vertices;
	int vertices_size; //number of vertices (not floating values)
	unsigned int VAO;
	unsigned int VBO;
	Shader* shaderProgram;
	glm::vec3 Position;

	RenderableObject(std::string path, Shader* _shaderProgram, glm::vec3 position);

	void draw();

	~RenderableObject();

};

#endif // !RENDERABLE_OBJECT_H
