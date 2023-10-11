#pragma once
#include <iostream>
#include <glm/glm/gtc/matrix_transform.hpp>

class Shader;
struct GLFWwindow;

class Water
{
	const float size = 50;
	const std::string vShader = "shaders/vertex\ shaders/WaterVertexShader.vert";
	const std::string fShader = "shaders/fragment\ shaders/WaterFragmentShader.frag";

	const int REFLECTION_WIDTH = 320;
	const int REFLECTION_HEIGHT = 180;

	const int REFRACTION_WIDTH = 1280;
	const int REFRACTION_HEIGHT = 720;
	GLFWwindow* window;

	glm::vec3 Position;
public:
	uint32_t VAO;
	uint32_t VBO;
	uint32_t ReflectionFrameBuffer;
	uint32_t ReflectionTexture;
	uint32_t ReflectionDepthBuffer;

	uint32_t RefractionFrameBuffer;
	uint32_t RefractionTexture;
	uint32_t RefractionDepthBuffer;


	float vertices[18] = { 
		-size,1,-size,
		-size,1,size,
		size,1,-size,
		size,1,size,
		size,1,-size,
		-size,1,size
	};
	Shader* shader;

	Water(GLFWwindow* _window, glm::vec3 position);
	void draw();
	uint32_t GenFBO();
	uint32_t CreateTextureAttachment(float width, float height);
	uint32_t CreateDepthTextureAttachment(float width, float height);
	uint32_t CreateDepthBufferAttachment(float width, float height);
	void initReflectionFBO();
	void initRefractionFBO();
	void bindFrameBuffer(uint32_t buffer, uint32_t width, uint32_t height);
	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindFrameBuffer();
	~Water();
};