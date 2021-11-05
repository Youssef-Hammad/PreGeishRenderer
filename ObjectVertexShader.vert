#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection*view*model*vec4(inPos,1.0);
	FragPos = vec3(model*vec4(inPos,1.0)); //Store fragment position with respect to world space
	TexCoord = inTexCoord;
	Normal = vec3(model*vec4(inNormal,1.0)); //Move normal to world space coordinates
}