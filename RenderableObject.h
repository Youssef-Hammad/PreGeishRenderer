#pragma once
#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "fast_obj.h"
#include "Shader.h"
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

	RenderableObject(std::string path, Shader* _shaderProgram);

	void draw();

	~RenderableObject();

};

#endif // !RENDERABLE_OBJECT_H
