#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "RenderableObject.h"
#include "Shader.h"
#include "Camera.h"
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

class Renderer
{
private:
	Shader* objectShaderProgram;

	void processInput(GLFWwindow* window);

public:
	GLFWwindow* window;
	std::vector<RenderableObject*> objects;
	bool initSuccess;
	Camera* camera;
	int Width;
	int Height;
	bool wireframe;

	//Camera Related Variables
	float deltaTime;
	float lastFrame;
	bool firstMouseMove;
	float lastX;
	float lastY;

	Renderer(int width, int height, std::string window_name);
	void render_scene();
	void AddObj(std::string path, glm::vec3 pos);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	~Renderer();
};

#endif // !RENDERER_H
