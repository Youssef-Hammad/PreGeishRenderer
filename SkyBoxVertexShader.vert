#version 330 core
layout (location = 0) in vec3 inPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 pos = projection*view*vec4(inPos,1.0);
	gl_Position = vec4(pos.x,pos.y,pos.w,pos.w);
	texCoords = vec3(inPos.x,inPos.y,-inPos.z);
}