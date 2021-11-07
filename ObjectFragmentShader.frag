#version 330 core

struct Material
{
	sampler2D diffuse;
	bool diffuseTextureExist;
	sampler2D specular;
	bool specularTextureExist;
	float shininess;
};

// The reason all 3 classes have the ambient/diffuse/specular repition is to have control over the lighting effects of different types of lighting

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;



void main()
{
	
}