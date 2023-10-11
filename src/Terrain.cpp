#include "Terrain.h"
#include "Texture.h"
#include "Shader.h"
#include <stb_image.h>
#include <glad/glad.h>

Terrain::Terrain(int gridX, int gridZ, std::string _texturePath, Shader* _shaderProgram, std::string heightMapPath)
{
	x = gridX * SIZE;
	z = gridZ * SIZE;
	texture = new Texture(GL_TEXTURE0, _texturePath, NORMAL_VEC, NORMAL_VEC, NORMAL_VEC, 2, SHINE);
	shaderProgram = _shaderProgram;
	generateTerrain(heightMapPath);
}

void Terrain::generateTerrain(std::string heightMapPath)
{
	int width, height, nrChannels;
	unsigned char* image = stbi_load(heightMapPath.c_str(), &width, &height, &nrChannels, 0);
	if (!image)
	{
		std::cout << "Failed to load height map" << std::endl;
		return;
	}

	int VERTEX_COUNT = height;

	int count = VERTEX_COUNT * VERTEX_COUNT;

	float* positions = new float[count * 3];
	float* normals = new float[count * 3];
	float* texCoords = new float[count * 2];

	int* indicies = new int[6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)];
	int vertIdx = 0;
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VERTEX_COUNT; j++)
		{
			positions[vertIdx * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			positions[vertIdx * 3 + 1] = getHeight(j, i, width, height, image);
			positions[vertIdx * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;

			glm::vec3 normal = calculateNormal(j, i, width, height, image);
			normals[vertIdx * 3] = normal.x;
			normals[vertIdx * 3 + 1] = normal.y;
			normals[vertIdx * 3 + 2] = normal.z;

			texCoords[vertIdx * 2] = (float)j / ((float)VERTEX_COUNT - 1);
			texCoords[vertIdx * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
			vertIdx++;
		}
	}

	int indicesIdx = 0;
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
	{
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indicies[indicesIdx++] = topLeft;
			indicies[indicesIdx++] = bottomLeft;
			indicies[indicesIdx++] = topRight;
			indicies[indicesIdx++] = topRight;
			indicies[indicesIdx++] = bottomLeft;
			indicies[indicesIdx++] = bottomRight;
		}
	}


	vertices = new float[indicesIdx * 8];
	verticesSize = indicesIdx * 8;
	int verticesIdx = 0;
	for (int i = 0; i < indicesIdx; i++)
	{
		vertices[verticesIdx++] = positions[3 * indicies[i]];
		vertices[verticesIdx++] = positions[3 * indicies[i] + 1];
		vertices[verticesIdx++] = positions[3 * indicies[i] + 2];

		vertices[verticesIdx++] = texCoords[2 * indicies[i]];
		vertices[verticesIdx++] = texCoords[2 * indicies[i] + 1];

		vertices[verticesIdx++] = normals[3 * indicies[i]];
		vertices[verticesIdx++] = normals[3 * indicies[i] + 1];
		vertices[verticesIdx++] = normals[3 * indicies[i] + 2];
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int stride_size = 8;

	glBufferData(GL_ARRAY_BUFFER, indicesIdx * stride_size * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] positions;
	delete[] texCoords;
	delete[] normals;
	delete[] indicies;

}

glm::vec3 Terrain::calculateNormal(int x, int z, int imgWidth, int imgHeight, unsigned char* image)
{
	float heightL = getHeight(x - 1, z, imgWidth, imgHeight, image);
	float heightR = getHeight(x + 1, z, imgWidth, imgHeight, image);
	float heightD = getHeight(x, z - 1, imgWidth, imgHeight, image);
	float heightU = getHeight(x, z + 1, imgWidth, imgHeight, image);
	glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f,heightD - heightU);
	normal = glm::normalize(normal);

	return normal;
}

float Terrain::getHeight(int x, int y, int imgWidth, int imgHeight, unsigned char* image)
{
	if (x < 0 || x >= imgWidth || y < 0 || y >= imgHeight)
	{
		//std::cout << "out of bounds" << std::endl;
		return 0;
	}
	float height = getRGB(x, y, image, imgHeight);

	//height += MAX_PIXEL_COLOUR / 2.0f;

	if (height > MAX_PIXEL_COLOUR / 2.0f || height < MAX_PIXEL_COLOUR / -2.0f)
		std::cout << "More than" << std::endl;

	//makes height in range of [-1, 1]
	height /= MAX_PIXEL_COLOUR / 2.0f;

	//makes height in range of [-MAX_HEIGHT, MAX_HEIGHT]
	height *= MAX_HEIGHT;

	// The -10 here because for some reason the terrain vertices y position is offseted by 10 (Will investigate why later)
	return height-10;
}

float Terrain::getRGB(int x, int y, unsigned char* image, int imgHeight)
{
	int idx = 3*(y * imgHeight + x);
	return float(image[idx]) * float(image[idx + 1]) * float(image[idx + 2]);
}

void Terrain::draw()
{
	shaderProgram->SetActive();
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f));
	shaderProgram->setMat4("model", model);
	texture->bind();
	shaderProgram->setInt("material.diffuseTex", 0);
	shaderProgram->setVec3("material.ambient", texture->material.ambient);
	shaderProgram->setVec3("material.diffuse", texture->material.diffuse);
	shaderProgram->setVec3("material.specular", texture->material.specular);
	shaderProgram->setFloat("material.shininess", texture->material.shininess);
	shaderProgram->setInt("material.specularTex", 1); //Will not sample from a specular map
	glDrawArrays(GL_TRIANGLES, 0, verticesSize);
	texture->unbind();
	glBindVertexArray(0);
}

Terrain::~Terrain()
{
	delete[] vertices;
	delete texture;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

