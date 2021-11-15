#pragma once
#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "fast_obj.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

class RenderableObject
{
private:
	void load_vertices();
	void load_textures();
public:
	fastObjMesh* mesh;
	float* vertices;
	int vertices_size; //number of vertices (not floating values)
	unsigned int VAO;
	unsigned int VBO;
	Shader* shaderProgram;
	glm::vec3 Position;
	std::vector<Texture*> textures;
	std::vector<std::pair<int, int> > texture_order; // each pair contains the number of consecutive vertices (first int) and the texture index they use (second int)

	RenderableObject(std::string path, Shader* _shaderProgram, glm::vec3 position);

	void draw();

	~RenderableObject();

};

#endif // !RENDERABLE_OBJECT_H
