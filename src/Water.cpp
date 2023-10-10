#include "Water.h"
#include <glad/glad.h>
#include "Shader.h"

Water::Water(glm::vec3 position) : Position(position)
{
	shader = new Shader(vShader, fShader);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	int stride = 3;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * stride * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Water::~Water()
{
	delete shader;
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
