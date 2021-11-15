#include "Renderer.h"

void Renderer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (!wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = true;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = false;
		}
	}
}

Renderer::~Renderer()
{
	delete camera;
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	glfwTerminate();
}

void Renderer::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->Process_Keyboard(FORWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->Process_Keyboard(BACKWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->Process_Keyboard(RIGHT, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->Process_Keyboard(LEFT, cameraSpeed);
}

void Renderer::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouseMove)
	{
		firstMouseMove = false;
		lastX = xpos;
		lastY = ypos;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera->Process_Mouse_Movement(xoffset, yoffset);
}

void Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->Process_Mouse_Scroll(yoffset);
}

Renderer::Renderer(int width, int height, std::string window_name)
{
	Width = width;
	Height = height;
	lastX = width / 2.0f;
	lastY = height / 2.0f;
	firstMouseMove = true;
	wireframe = false;
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	stbi_set_flip_vertically_on_load(true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Pre Geish Renderer", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		initSuccess = false;
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD!" << std::endl;
		glfwTerminate();
		initSuccess = false;
		return;
	}
	initSuccess = true;

	glViewport(0, 0, width, height);

	std::string objectVertPath = "C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\objectVertexShader.vert";
	std::string objectFragPath = "C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\ObjectFragmentShader.frag";

	objectShaderProgram = new Shader(objectVertPath, objectFragPath);

	glEnable(GL_DEPTH_TEST);

}

void Renderer::render_scene()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	processInput(window);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objectShaderProgram->SetActive();
	
	glm::mat4 view = camera->GetViewMatrix();
	objectShaderProgram->setMat4("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
	objectShaderProgram->setMat4("projection", projection);

	for (int i = 0; i < objects.size(); i++)
	{
		std::cout << "Drawing" << std::endl;
		objects[i]->draw();
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void Renderer::AddObj(std::string path, glm::vec3 pos)
{
	RenderableObject* temp = new RenderableObject(path, objectShaderProgram, pos);
	objects.push_back(temp);
}
