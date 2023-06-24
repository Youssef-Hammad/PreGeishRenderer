#version 330 core
layout (location = 0) in vec3 inPos;

out vec3 texCoords;

// We don't need a model matrix here since the sky is always moving with the CAMERA
uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 pos = projection*view*vec4(inPos,1.0);
	// pos.w is always 1, which will result in the z component always being 1
	// Since the depth buffer takes the value from the z component (which is 1 in this case)
	// it will ensure the skybox is always rendered behind all rendered objects
	gl_Position = vec4(pos.x,pos.y,pos.w,pos.w);

	// the -z here flips the skybox to comply with opengl's right-handed system
	// Check texture implementation for skybox law rage3 mel gesh w nasy
	texCoords = vec3(inPos.x,inPos.y,-inPos.z);
}