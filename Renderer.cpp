#include "Renderer.h"

Renderer::Renderer(int width, int height, std::string name)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
}

void Renderer::render_scene()
{
	glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/*for (int i = 0; i < objects.size(); i++)
	{
		objects[i].draw();
	}*/
}

Renderer::~Renderer()
{
	glfwTerminate();
}
