#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "RenderableObject.h"
#include "Camera.h"
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5f * renderer.deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		renderer.camera->Process_Keyboard(FORWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		renderer.camera->Process_Keyboard(BACKWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		renderer.camera->Process_Keyboard(RIGHT, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		renderer.camera->Process_Keyboard(LEFT, cameraSpeed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (renderer.firstMouseMove)
	{
		renderer.lastX = xpos;
		renderer.lastY = ypos;
		renderer.firstMouseMove = false;
	}

	float xoffset = xpos - renderer.lastX;
	float yoffset = renderer.lastY - ypos;

	renderer.lastX = xpos;
	renderer.lastY = ypos;

	renderer.camera->Process_Mouse_Movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	renderer.camera->Process_Mouse_Scroll(yoffset);
}

int main()
{

	if (!renderer.initSuccess)
	{
		std::cout << "Failed to initialize renderer" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(renderer.window, key_callback);
	glfwSetCursorPosCallback(renderer.window, mouse_callback);
	glfwSetScrollCallback(renderer.window, scroll_callback);

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	renderer.AddObj("E:\\3D-Models\\RubiksCube\\rubikscube.obj", glm::vec3(0.0f, 0.0f, 0.0f));
	//renderer.AddObj("E:\\3D-Models\\Antylamon Snow\\Antylamon Snow.obj", glm::vec3(10.0f, 0.0f, 0.0f));
	//renderer.AddObj("E:\\3D-Models\\Digimon Tai\\Item1\\digimon-digital-monsters-tai.obj", glm::vec3(0.0f,10.0f,0.0f));
	//renderer.AddObj("E:\\3D-Models\\Electro Cicin Mage\\Electro Cicin Mage.obj", glm::vec3(0.0f, 0.0f, 0.0f));
	//renderer.AddObj("E:\\3D-Models\\Wargreymon\\WarGreymon.obj", glm::vec3(0.0f, 0.0f, 0.0f));
	//renderer.AddObj("E:\\3D-Models\\Kirby\\Kirby.obj", pos);
	//renderer.AddObj("E:\\3D-Models\\Kirby Trophy\\Kirby.obj", pos);

	while (!glfwWindowShouldClose(renderer.window))
	{

		renderer.render_scene();
	}

	glfwTerminate();


	return 0;
}