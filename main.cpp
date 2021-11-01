#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include <iostream>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main()
{
	/*fastObjMesh* mesh = fast_obj_read("E:\\3D-Models\\RubiksCube\\rubikscube.obj");

	for (int i = 0; i < 20; i++)
	{
		std::cout << "Index " << i + 1 << ": " << mesh->indices[i].p << "/" << mesh->indices[i].t << "/" << mesh->indices[i].n << "\n";
		std::cout << "Position: " << mesh->positions[mesh->indices[i].p*3] << "\n";
		std::cout << "TexCoords: " << mesh->texcoords[mesh->indices[i].t*2] << "\n";
		std::cout << "Normal: " << mesh->normals[mesh->indices[i].n*3] << "\n";
		std::cout << "\n";
	}

	fast_obj_destroy(mesh);*/

	Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT, "Pre Geish Renderer");

	if (renderer.window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(renderer.window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	while (!glfwWindowShouldClose(renderer.window))
	{
		renderer.render_scene();

		glfwSwapBuffers(renderer.window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}