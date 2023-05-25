#pragma once

#ifndef INITIALIZER_H
#define INITIALIZER_H


#include "Renderer.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT, "Pre Geish Renderer");

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (!renderer.wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			renderer.wireframe = true;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			renderer.wireframe = false;
		}
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (renderer.firstMouseMove)
	{
		renderer.lastX = (float)xpos;
		renderer.lastY = (float)ypos;
		renderer.firstMouseMove = false;
	}

	float xoffset = (float)xpos - renderer.lastX;
	float yoffset = renderer.lastY - (float)ypos;

	renderer.lastX = (float)xpos;
	renderer.lastY = (float)ypos;

	renderer.camera->Process_Mouse_Movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	renderer.camera->Process_Mouse_Scroll((float)yoffset);
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Initialize()
{
	if (!renderer.initSuccess)
	{
		std::cout << "Failed to initialize renderer" << std::endl;
		return;
	}


	glfwSetKeyCallback(renderer.window, key_callback);
	glfwSetCursorPosCallback(renderer.window, mouse_callback);
	glfwSetScrollCallback(renderer.window, scroll_callback);
	glfwSetFramebufferSizeCallback(renderer.window, frame_buffer_size_callback);
}

bool WindowIsOpen()
{
	return !glfwWindowShouldClose(renderer.window);
}

void Terminate()
{
	glfwTerminate();
}

#endif