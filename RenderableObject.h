#pragma once

#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

#include "fast_obj.h"
#include <iostream>
#include <vector>
#include "Texture.h"

class RenderableObject
{
private:
	void load_vertices();
	void load_textures();
public:
	fastObjMesh* mesh;
	int vertixCount;
	float* vertices;
	unsigned int VAO;
	unsigned int VBO;

	std::vector<Texture> textures;
	std::vector<std::pair<int, int> > texture_order; // each pair contains the number of consecutive vertices (first int) and the texture index they use (second int)


	RenderableObject(std::string path);
	void draw();

	~RenderableObject();
};

#endif // !RENDERABLEOBJECT_H