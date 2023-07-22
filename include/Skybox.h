#pragma once

#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>

// CONSTANTS

const std::string vertPath = "shaders/vertex\ shaders/SkyBoxVertexShader.vert";
const std::string fragPath = "shaders/fragment\ shaders/SkyBoxFragmentShader.frag";

const float skyboxvertices[] =
{
	-1.0f,	-1.0f,	1.0f,
	1.0f,	-1.0f,	1.0f,
	1.0f,	-1.0f,	-1.0f,
	-1.0f,	-1.0f,	-1.0f,
	-1.0f,	1.0f,	1.0f,
	1.0f,	1.0f,	1.0f,
	1.0f,	1.0f,	-1.0f,
	-1.0f,	1.0f,	-1.0f
};

const unsigned int skyboxIndicies[] =
{
	//Right
	1,2,6,
	6,5,1,

	//Left
	0,4,7,
	7,3,0,

	//Top
	4,5,6,
	6,7,4,

	//Bottom
	0,3,2,
	2,1,0,

	//Back
	0,1,5,
	5,4,0,

	//Front
	3,7,6,
	6,2,3
};

const std::string cubeMaps[6] = {
	"test_resources\\skybox\\right.jpg",
	"test_resources\\skybox\\left.jpg",
	"test_resources\\skybox\\top.jpg",
	"test_resources\\skybox\\bottom.jpg",
	"test_resources\\skybox\\front.jpg",
	"test_resources\\skybox\\back.jpg"
};

// CLASS DECLARATION

class Shader;
class Camera;

class Skybox
{
private:
	Shader* shaderProgram;
	unsigned int VAO, VBO, EBO, Texture;
	Camera* camera;
	int Width;
	int Height;
	


public:
	Skybox(Camera* cam, int width, int height);
	~Skybox();

	void draw();
};

#endif // !SKYBOX_H
