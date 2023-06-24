#pragma once

#ifndef SKYBOX_H
#define SKYBOX_H

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
