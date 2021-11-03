#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include "RenderableObject.h"
#include <vector>
#include <GLFW/glfw3.h>

class Renderer
{
public:
	GLFWwindow* window;
	std::vector<RenderableObject*> objects;

	Renderer(int width, int height, std::string name);
	void render_scene();
	void AddObj(std::string path);


	~Renderer();
};

#endif // !RENDERER_H


