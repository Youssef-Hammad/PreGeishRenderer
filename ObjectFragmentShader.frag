#version 330 core

struct Material
{
	sampler2D diffuse;
	//bool diffuseTextureExist;
	sampler2D specular;
	//bool specularTextureExist;
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

out vec4 FragColor;

#define pointLightCnt 4

uniform DirectionalLight directionalLight;
uniform PointLight pointLight[pointLightCnt];

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 FragPos, vec3 normal, vec3 viewDir);
//vec3 calculateSpotLight(SpotLight light, vec3 FragPos, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(-FragPos);
	vec3 result = calculateDirectionalLight(directionalLight, norm, viewDir);

	for(int i = 0; i < pointLightCnt; i++)
	{
		result += calculatePointLight(pointLight[i], FragPos, norm, viewDir);
	}

	//result += calculateSpotLight(spotLight, FragPos, norm, viewDir);

	FragColor = vec4(result,1.0);

}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	//if(material.diffuseTextureExist)
	ambient = light.ambient*vec3(texture(material.diffuse,TexCoord)).rgb;

	vec3 lightDir = normalize(-light.direction);
	float diffAngle = max(dot(normal,lightDir),0.0); //Calculate angle between normal and light direction through dot product
	diffuse = light.diffuse*diffAngle*vec3(texture(material.diffuse,TexCoord)).rgb;


	vec3 reflectDir = reflect(-lightDir,normal); // reflection of the light with respect to the fragment's normal

	// Calculates the angle between viewDir and the direction of reflected light (reflectDir)
	// Then powered by material.shininess to determine how much the light on the reflected spot should be scattered (the higher shininess value the less scattered it will be)
	float specularAngle = pow(max(dot(viewDir,reflectDir),0.0),material.shininess); 
	specular = light.specular*specularAngle*vec3(texture(material.specular,TexCoord)).rgb;

	return ambient+diffuse+specular;
}


vec3 calculatePointLight(PointLight light, vec3 FragPos, vec3 normal, vec3 viewDir)
{
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	ambient = light.ambient*vec3(texture(material.diffuse,TexCoord)).rgb;

	vec3 lightDir = normalize(light.position-FragPos);
	float diffAngle = max(dot(normal,lightDir),0.0);
	diffuse = light.diffuse*diffAngle*vec3(texture(material.diffuse,TexCoord)).rgb;

	vec3 reflectDir = reflect(-lightDir,normal);
	float specularAngle = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	specular = light.specular*specularAngle*vec3(texture(material.specular,TexCoord)).rgb;

	float dist = length(light.position-FragPos);
	float attenuation = 1.0/(light.constant+light.linear*dist+light.quadratic*(dist*dist));

	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;

	return ambient+diffuse+specular;
	
}