#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec3 inNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection*view*model*vec4(inPos,1.0);
	TexCoords = inTexCoords;
	Normal = vec3(view*model*vec4(inNormal,1.0)); // Move normal to view space coordinates
	FragPos = vec3(view*model*vec4(inPos,1.0)); // Move fragment position to view space so the viewPos is always (0,0,0) in the fragment shader
}