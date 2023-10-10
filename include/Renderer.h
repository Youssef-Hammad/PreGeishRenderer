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



struct GLFWwindow;

typedef unsigned int GLenum;

class Renderer
{
private:
	Shader* objectShaderProgram;
	Shader* terrainShaderProgram;
	Shader* skyboxShaderProgram;
	GLenum currentTextureNumber;
	std::string Window_Name;

	const std::string RO_vShader = "shaders/vertex\ shaders/ObjectVertexShader.vert";
	const std::string RO_fShader = "shaders/fragment\ shaders/ObjectFragmentShader.frag";
	const std::string Ter_vShader = "shaders/vertex\ shaders/TerrainVertexShader.vert";
	const std::string Ter_fShader = "shaders/fragment\ shaders/TerrainFragmentShader.frag";

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
	bool renderSkyBox;

	Renderer(int width, int height, std::string window_name);
	void render_scene();
	void ToggleSkyBox();
	void AddObj(std::string path, glm::vec3 pos);
	void AddTerrain();
	inline void DrawObj();
	inline void DrawTerrain();
	inline void CalculateFrames();

	~Renderer();
};

#endif // !RENDERER_H
