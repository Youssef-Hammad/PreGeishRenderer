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
	
	//The multiplication here is for tiling the texture, the higher the number, the more tiles are made
	//Once the coordinates go over 1.0 OpenGL starts again from 0.0 (more like the modulo concept)
	//Therefore, it makes the fairly small texture get tiled multiple times over the terrain
	//and thus looking better than having it stretched over the entire terrain
	TexCoords = inTexCoords*100.0;

	// The transpose(inverse()) part is to correctly calculate the normal coordinates even when a non-uniform scaling occurs. (I don't fully understand the math behind it)
	Normal = mat3(transpose(inverse(model)))*inNormal; // Move normal to world space coordinates
	FragPos = vec3(model*vec4(inPos,1.0)); // Move fragment position to world space
}