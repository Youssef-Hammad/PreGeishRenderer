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
	FragPos = vec3(view*model*vec4(inPos,1.0)); //Store fragment position with respect to view space so the viewPos is (0,0,0)
	TexCoord = inTexCoord;
	Normal = vec3(view*model*vec4(inNormal,1.0)); //Move normal to view space coordinates
}