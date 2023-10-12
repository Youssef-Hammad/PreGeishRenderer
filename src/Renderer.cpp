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
#include "Water.h"

Renderer::~Renderer()
{
	delete camera;
	delete objectShaderProgram;
	delete terrainShaderProgram;
	delete skyboxShaderProgram;
	delete skybox;
	delete water;
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

	camera->Process_Keyboard(window, deltaTime);
}

Renderer::Renderer(int width, int height, std::string window_name)
	: Width(width), Height(height), Window_Name(window_name)
{
	wireframe = false;
	deltaTime = 0.0f;
	lastFrame = 0.0f;
	currentTextureNumber = GL_TEXTURE0;
	renderSkyBox = false;

	camera = new Camera(width,height);

	stbi_set_flip_vertically_on_load(true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(Width, Height, Window_Name.c_str(), NULL, NULL);

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

	skybox = new Skybox(camera,Width,Height);
	water = new Water(window, glm::vec3(400,-1,400));
	reflectionplane.w = water->height * -1;
	refractionplane.w = water->height;

	terrainShaderProgram = new Shader(Ter_vShader, Ter_fShader);

	objectShaderProgram = new Shader(RO_vShader, RO_fShader);
	objectShaderProgram->SetActive();
	objectShaderProgram->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, 1.0f));
	objectShaderProgram->setVec3("dirLight.ambient", glm::vec3(.5f, .5f, .5f));
	objectShaderProgram->setVec3("dirLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
	objectShaderProgram->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	objectShaderProgram->setVec4("plane", reflectionplane);

	terrainShaderProgram->SetActive();
	terrainShaderProgram->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, 1.0f));
	terrainShaderProgram->setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	terrainShaderProgram->setVec3("dirLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
	terrainShaderProgram->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	terrainShaderProgram->setVec4("plane", reflectionplane);



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
	glEnable(GL_CLIP_DISTANCE0);
	CalculateFrames();
	processInput(window);
	glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float distance = 2 * (camera->Position.y-water->height);
	camera->Position.y -= distance;
	camera->invertPitch();
	currentPlane = reflectionplane;
	water->bindReflectionFrameBuffer(reflectionplane);
	DrawObj();
	DrawTerrain();
	DrawWater();
	camera->Position.y += distance;
	camera->invertPitch();

	currentPlane = refractionplane;
	water->bindRefractionFrameBuffer(refractionplane);
	DrawObj();
	DrawTerrain();
	DrawWater();

	if (renderSkyBox)
		skybox->draw();
	
	water->unbindFrameBuffer();
	glDisable(GL_CLIP_DISTANCE0);

	currentPlane = glm::vec4(0, -1, 0, 100000);
	DrawObj();
	DrawTerrain();
	DrawWater();

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

inline void Renderer::DrawObj()
{
	objectShaderProgram->SetActive();
	objectShaderProgram->setVec4("plane", currentPlane);
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
}

inline void Renderer::DrawTerrain()
{
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)Width / (float)Height, 0.1f, 1000.0f);
	terrainShaderProgram->SetActive();
	terrainShaderProgram->setVec4("plane", currentPlane);
	terrainShaderProgram->setMat4("view", view);

	projection = glm::perspective(glm::radians(camera->Zoom), (float)Width / (float)Height, 0.1f, 1000.0f);
	terrainShaderProgram->setMat4("projection", projection);

	terrainShaderProgram->setVec3("viewPos", camera->Position);
	for (int i = 0; i < terrains.size(); i++)
	{
		terrains[i]->draw();
	}
}

inline void Renderer::CalculateFrames()
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
	std::string newTitle = Window_Name + " - " + FPS + " FPS    /    " + ms + " ms";
	glfwSetWindowTitle(window, newTitle.c_str());
}

inline void Renderer::DrawWater()
{
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)Width / (float)Height, 0.1f, 1000.0f);
	water->shader->SetActive();
	water->shader->setMat4("view", view);
	water->shader->setMat4("projection", projection);
	water->draw();
}
