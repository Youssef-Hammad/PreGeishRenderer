#pragma once

#ifndef SKYBOX_H
#define SKYBOX_H

class Skybox
{
private:
	class Shader* shaderProgram;
	unsigned int VAO, VBO, EBO, Texture;
	class Camera* camera;
	int Width;
	int Height;



public:
	Skybox(class Camera* cam, int width, int height);
	~Skybox();

	void draw();
};

#endif // !SKYBOX_H
