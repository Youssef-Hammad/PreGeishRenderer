#version 330 core


struct Material
{
	bool renderDiffTexture;
	bool renderSpecTexture;
	sampler2D diffuseTex;
	sampler2D specularTex;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// inputs from vertex shader
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;


out vec4 FragColor;


// uniforms
uniform DirectionalLight dirLight;
uniform vec3 viewPos;
uniform Material material;

// Functions
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main()
{
	// Remember to normalize everything
	vec3 norm = normalize(Normal); 
	//Vector pointing towards the fragment
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 result = calculateDirectionalLight(dirLight, norm, viewDir);

	FragColor = vec4(result,1.0);
	//FragColor = texture(material.diffuseTex,TexCoords);
}

// In case I don't remember why this is done
// Read the Lighting section starting from here: https://learnopengl.com/Lighting/Basic-Lighting
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	// Phong lighting initializations
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);


	ambient = (light.ambient*material.ambient)*vec3(texture(material.diffuseTex,TexCoords)).rgb;

	// Pointing towards light source (hence, the negative sign)
	vec3 lightDir = normalize(-light.direction);
	// Calculate angle between normal and light direction through dot product
	float diffAngle = max(dot(normal,lightDir),0.0);

	// Determine diffuse effect by multiplying light's diffuse with the angle (Scalar) between the normal and light direction
	// Then multiplying the result with the material's diffuse and finally with the texture color
	diffuse = ((light.diffuse*diffAngle)*material.diffuse)*vec3(texture(material.diffuseTex,TexCoords)).rgb;

	// reflection of the light with respect to the fragment's normal
	vec3 reflectDir = reflect(-lightDir,normal);

	// Calculates the angle between viewDir and the direction of reflected light (reflectDir)
	// Then powered by material.shininess to determine how much the light on the reflected spot should be scattered (the higher shininess value the less scattered it will be)
	
	// This is to handle when Shininess is equal to 0.
	// Reason for this handling is that GLSL's pow produces undefined result when the base <= 0 and power <= 0
	float specularAngle;
	if(material.shininess == 0)
		specularAngle=1;
	else
		specularAngle = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	specular = ((light.specular*specularAngle)*material.specular)*vec3(texture(material.specularTex,TexCoords)).rgb;


	return ambient+diffuse+specular;

}