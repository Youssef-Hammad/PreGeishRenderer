#include "Skybox.h"
#include <string>
#include "Shader.h"
#include "Camera.h"
#include <stb_image.h>

Skybox::Skybox(Camera* cam, int width, int height)
{
	std::string vertPath = "shaders/vertex\ shaders/SkyBoxVertexShader.vert";
	std::string fragPath = "shaders/fragment\ shaders/SkyBoxFragmentShader.frag";
	shaderProgram = new Shader(vertPath, fragPath);
	camera = cam;
	Width = width;
	Height = height;


	std::string cubeMaps[] =
	{
		"test_resources\\skybox\\right.jpg",
		"test_resources\\skybox\\left.jpg",
		"test_resources\\skybox\\top.jpg",
		"test_resources\\skybox\\bottom.jpg",
		"test_resources\\skybox\\front.jpg",
		"test_resources\\skybox\\back.jpg"
	};

	float skyboxvertices[] =
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

	unsigned int skyboxIndicies[] =
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


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxvertices), &skyboxvertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndicies), &skyboxIndicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shaderProgram->SetActive();
	shaderProgram->setInt("skyboxTex", 0);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < sizeof(cubeMaps) / sizeof(std::string); i++)
	{
		int width, height, nrChannels;
		unsigned char* img = stbi_load(cubeMaps[i].c_str(), &width, &height, &nrChannels, 0);
		if (img != nullptr)
		{
			// Cube maps textures are expected to start from top left instead of bottom left
			// That's why we disable flipping
			stbi_set_flip_vertically_on_load(false);

			// Order of the cube map sides (and their names)
			// GL_TEXTURE_CUBE_MAP_POSITIVE_X	right
			// GL_TEXTURE_CUBE_MAP_NEGATIVE_X	left
			// GL_TEXTURE_CUBE_MAP_POSITIVE_Y	top
			// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	bottom
			// GL_TEXTURE_CUBE_MAP_POSITIVE_Z	front
			// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	back
			// For some reason the front (which is expected to be the negative z)
			// is the positive z, meaning cube maps are made in left-handed system
			// No idea why, but that's how it is


			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		}
		else
			std::cout << "Failed to load: " << cubeMaps[i] << std::endl;

		stbi_image_free(img);

	}

	stbi_set_flip_vertically_on_load(true);

}

Skybox::~Skybox()
{
	delete shaderProgram;

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &Texture);

}

void Skybox::draw()
{
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	shaderProgram->SetActive();

	glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.f);

	shaderProgram->setMat4("view", view);
	shaderProgram->setMat4("projection", projection);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}
