#include "Renderer.h"
#include "Skybox.h"
#include "RenderableObject.h"
#include "Terrain.h"
#include "Shader.h"
#include "Camera.h"
#include <stb_image.h>
#include <glad/glad.h>
#include "Timer.h"
#include <GLFW/glfw3.h>

Renderer::~Renderer()
{
	delete camera;
	delete objectShaderProgram;
	delete terrainShaderProgram;
	delete skyboxShaderProgram;
	glDeleteBuffers(1, &SkyBoxVBO);
	glDeleteBuffers(1, &SkyBoxEBO);
	glDeleteVertexArrays(1, &SkyBoxVAO);
	glDeleteTextures(1, &SkyBoxTexture);
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	for (int i = 0; i < terrains.size(); i++)
		delete terrains[i];
	glfwTerminate();
}

void Renderer::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 1.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = deltaTime * 0.6f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraSpeed = deltaTime * 20.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->Process_Keyboard(FORWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->Process_Keyboard(BACKWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->Process_Keyboard(RIGHT, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->Process_Keyboard(LEFT, cameraSpeed);
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
	currentTextureNumber = GL_TEXTURE0;
	renderSkyBox = false;

	camera = new Camera();

	stbi_set_flip_vertically_on_load(true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(Width, Height, window_name.c_str(), NULL, NULL);

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

	glViewport(0, 0, Width, Height);

	std::string objectVertPath = "shaders/vertex\ shaders/ObjectVertexShader.vert";
	std::string objectFragPath = "shaders/fragment\ shaders/ObjectFragmentShader.frag";

	std::string terrainVertPath = "shaders/vertex\ shaders/TerrainVertexShader.vert";
	std::string terrainFragPath = "shaders/fragment\ shaders/TerrainFragmentShader.frag";

	//std::string skyboxVertPath = "shaders/vertex\ shaders/SkyBoxVertexShader.vert";
	//std::string skyboxFragPath = "shaders/fragment\ shaders/SkyBoxFragmentShader.frag";

	skybox = new Skybox(camera,Width,Height);

	/*skyboxShaderProgram = new Shader(skyboxVertPath, skyboxFragPath);

	{
		Timer timer;
		InitSkyBox();
		std::cout << "Skybox loading time:\n";
	}*/
	//std::cout << "\n\n";

	terrainShaderProgram = new Shader(terrainVertPath, terrainFragPath);

	objectShaderProgram = new Shader(objectVertPath, objectFragPath);
	objectShaderProgram->SetActive();
	objectShaderProgram->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, 1.0f));
	objectShaderProgram->setVec3("dirLight.ambient", glm::vec3(.5f, .5f, .5f));
	objectShaderProgram->setVec3("dirLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
	objectShaderProgram->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	terrainShaderProgram->SetActive();
	objectShaderProgram->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, 1.0f));
	objectShaderProgram->setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	objectShaderProgram->setVec3("dirLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
	objectShaderProgram->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// Enabling depth testing for 3D movement
	glEnable(GL_DEPTH_TEST);

	// Enabling face culling to avoid rendering triangles if they're viewed from 
	glEnable(GL_CULL_FACE);
	// The face we want to cull (which will be the front most of the time. I don't know why, figured this out by trial and error)
	glCullFace(GL_FRONT);
	// Uses clockwise standard for face culling. Meaning that if the vertices are viewed from angle that makes them arranged in a counter-clockwise manner, they get culled
	glFrontFace(GL_CW);

}

void Renderer::render_scene()
{
	float currentFrame = glfwGetTime();
	// Time taken to render a single frame calculated in seconds
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Calculating the number of frames per second (law el geish nasak leh bte3mel 1.0/deltaTime, basically cross multiplication)
	// Example: it takes 0.008s to render 1 frame, so it will render (1/0.008) frames per second
	std::string FPS = std::to_string(1.0 / deltaTime);
	// Converting deltaTime to milliseconds
	std::string ms = std::to_string(deltaTime * 1000);
	std::string newTitle = "Pre Geish Renderer - " + FPS + "FPS    /    " + ms + "ms";
	glfwSetWindowTitle(window, newTitle.c_str());
	processInput(window);
	glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objectShaderProgram->SetActive();
	
	glm::mat4 view = camera->GetViewMatrix();
	objectShaderProgram->setMat4("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)Width / (float)Height, 0.1f, 1000.0f);
	objectShaderProgram->setMat4("projection", projection);

	objectShaderProgram->setVec3("viewPos", camera->Position);

	for (int i = 0; i < objects.size(); i++)
	{
		//std::cout << "Drawing" << std::endl;
		objects[i]->draw();
	}

	terrainShaderProgram->SetActive();
	terrainShaderProgram->setMat4("view", view);

	projection = glm::perspective(glm::radians(camera->Zoom), (float)Width / (float)Height, 0.1f, 1000.0f);
	terrainShaderProgram->setMat4("projection", projection);

	terrainShaderProgram->setVec3("viewPos", camera->Position);

	for (int i = 0; i < terrains.size(); i++)
	{
		terrains[i]->draw();
	}

	if (renderSkyBox)
		skybox->draw();

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void Renderer::AddObj(std::string path, glm::vec3 pos)
{
	RenderableObject* temp = new RenderableObject(path, objectShaderProgram, pos);
	objects.push_back(temp);
}


void Renderer::ToggleSkyBox()
{
	renderSkyBox = !renderSkyBox;
}

void Renderer::AddTerrain()
{
	Terrain* terrain = new Terrain(0,0,"test_resources\\grass.png",terrainShaderProgram,"test_resources\\JnBQ0y9Q.png");
	terrains.push_back(terrain);
}
