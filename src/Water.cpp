#include "Water.h"
#include <glad/glad.h>
#include "Shader.h"
#include <GLFW/glfw3.h>

Water::Water(GLFWwindow* _window, glm::vec3 position) 
	: window(_window), Position(position)
{
	shader = new Shader(vShader, fShader);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	GenFBO();
	CreateTextureAttachment(1, 1);

	int stride = 3;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * stride * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	initReflectionFBO();
	initRefractionFBO();
}

Water::~Water()
{
	delete shader;
	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteFramebuffers(1, &ReflectionFrameBuffer);
	glDeleteFramebuffers(1, &RefractionFrameBuffer);
	glDeleteTextures(1, &ReflectionTexture);
	glDeleteTextures(1, &RefractionTexture);
	glDeleteRenderbuffers(1, &ReflectionDepthBuffer);
	glDeleteRenderbuffers(1, &RefractionDepthBuffer);
}

void Water::draw()
{
	shader->SetActive();
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES,0,6);
	glBindVertexArray(0);
}

uint32_t Water::GenFBO()
{
	uint32_t FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return FBO;
}

uint32_t Water::CreateTextureAttachment(float width, float height)
{
	uint32_t textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);
	return textureID;

}

uint32_t Water::CreateDepthTextureAttachment(float width, float height)
{
	uint32_t depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	return depthTexture;
}

uint32_t Water::CreateDepthBufferAttachment(float width, float height)
{
	uint32_t depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}

void Water::initReflectionFBO()
{
	ReflectionFrameBuffer = GenFBO();
	ReflectionTexture = CreateTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	ReflectionDepthBuffer = CreateDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbindFrameBuffer();
}

void Water::initRefractionFBO()
{
	RefractionFrameBuffer = GenFBO();
	RefractionTexture = CreateTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	RefractionDepthBuffer = CreateDepthBufferAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbindFrameBuffer();
}

void Water::bindFrameBuffer(uint32_t buffer, uint32_t width, uint32_t height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glViewport(0, 0, width, height);
}

void Water::bindReflectionFrameBuffer()
{
	bindFrameBuffer(ReflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void Water::bindRefractionFrameBuffer()
{
	bindFrameBuffer(RefractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void Water::unbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
}
