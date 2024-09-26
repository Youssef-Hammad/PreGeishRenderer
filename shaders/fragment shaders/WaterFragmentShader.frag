#version 330 core

in vec4 clipSpace;

out vec4 FragColor;

uniform sampler2D reflectTex;
uniform sampler2D refractTex;

void main()
{
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0+0.5;
	vec2 refractTexCoords = vec2(ndc.x,ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x,-ndc.y);
	vec4 reflect = texture(reflectTex,reflectTexCoords);
	vec4 refract = texture(refractTex,refractTexCoords);

	FragColor = mix(reflect,refract,0.5);
}