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
	renderer.AddObj("E:\\3D-Models\\Antylamon Snow\\Antylamon Snow.obj", glm::vec3(3.0f, 0.0f, 0.0f));
	renderer.AddObj("E:\\3D-Models\\Digimon Tai\\Item1\\digimon-digital-monsters-tai.obj", glm::vec3(6.0f,0.0f,0.0f));
	renderer.AddObj("E:\\3D-Models\\Electro Cicin Mage\\Electro Cicin Mage.obj", glm::vec3(9.0f, 0.0f, 0.0f));
	renderer.AddObj("E:\\3D-Models\\AmongUs\\Item1\\untitled.obj", glm::vec3(12.0f, 0.0f, 0.0f));
	//renderer.AddObj("E:\\3D-Models\\Wargreymon\\WarGreymon.obj", glm::vec3(14.0f, 0.0f, 0.0f));
	renderer.AddTerrain();

	renderer.ToggleSkyBox();

	//Texture tex(GL_TEXTURE0, "C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\test_resources\\JnBQ0y9Q.png", glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 2, 32);

	while (!glfwWindowShouldClose(renderer.window))
	{

		renderer.render_scene();
	}

	glfwTerminate();


	return 0;
}