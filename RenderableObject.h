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
	void load_vertices(); // load vertices from mesh to vertices array
	void load_textures(); // load all the textures
public:
	fastObjMesh* mesh; // object from fast_obj.h that stores all the info about the 3D object
	float* vertices; // array containing vertex data
	int vertices_size; //number of vertices (not floating values)
	unsigned int VAO;
	unsigned int VBO;
	Shader* shaderProgram;
	glm::vec3 Position; // Object's position in the world
	std::vector<Texture> textures;
	std::vector<std::pair<int, int> > texture_order; // each pair contains the number of consecutive vertices (first int) and the texture index they use (second int)

	RenderableObject(std::string path, Shader* _shaderProgram, glm::vec3 position);

	void draw();

	~RenderableObject();

};

#endif // !RENDERABLE_OBJECT_H
