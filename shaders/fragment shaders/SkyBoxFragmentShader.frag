#version 330 core

out vec4 Frag_Color;

in vec3 texCoords;

uniform samplerCube skyboxTex;

void main()
{
	Frag_Color = texture(skyboxTex,texCoords);
}