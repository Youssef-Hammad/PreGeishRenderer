#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inCol;

out vec3 color;

void main()
{
	gl_Position = vec4(inPos,1.0);
	color = inCol;
}