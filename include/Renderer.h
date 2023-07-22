#pragma once

#ifndef RENDERER_H
#define RENDERER_H


#include <iostream>
#include <vector>
#include <glm/glm/glm.hpp>

class Shader;
class Camera;
class Skybox;
class Terrain;
class RenderableObject;
class vector;

struct GLFWwindow;

typedef unsigned int GLenum;

class Renderer
{
private:
	Shader* objectShaderProgram;
	Shader* terrainShaderProgram;
	Shader* skyboxShaderProgram;
	GLenum currentTextureNumber;
	unsigned int SkyBoxVAO, SkyBoxVBO, SkyBoxEBO, SkyBoxTexture;

	void processInput(GLFWwindow* window);

public:
	GLFWwindow* window;
	std::vector<RenderableObject*> objects;
	std::vector<Terrain*> terrains;
	Skybox* skybox;
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
	bool renderSkyBox;

	Renderer(int width, int height, std::string window_name);
	void render_scene();
	void ToggleSkyBox();
	void AddObj(std::string path, glm::vec3 pos);
	void AddTerrain();

	~Renderer();
};

#endif // !RENDERER_H
