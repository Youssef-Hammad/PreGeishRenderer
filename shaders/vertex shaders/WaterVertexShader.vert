#version 330 core

layout (location = 0) in vec3 inPos;

out vec2 textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection*view*model*vec4(inPos,1.0);
	textureCoords = vec2(inPos.x/2.0 + 0.5, inPos.y/2.0 + 0.5);
}