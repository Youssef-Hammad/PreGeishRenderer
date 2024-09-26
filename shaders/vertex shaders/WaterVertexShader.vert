#version 330 core

layout (location = 0) in vec3 inPos;

out vec4 clipSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	clipSpace = projection*view*model*vec4(inPos,inPos,1.0);
	gl_Position = clipSpace;
}