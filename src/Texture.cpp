#include "Texture.h"

Texture::Texture(GLenum TextureNumber, std::string path, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, int illum, float shine)
{
	material = { amb,diff,spec,illum,shine };

	glGenTextures(1, &ID);
	texture_number = TextureNumber;
	texPath = path;

	//std::cout << "Texture Number & Static Texture Number: " << texture_number << " " << current_texture_number << std::endl;

	glActiveTexture(texture_number);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* image_data = stbi_load(path.c_str(), &width, &height, &nrChannels,0);
	//if loaded successfully
	if (image_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (nrChannels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(image_data);
	}
	else
	{
		std::cout << "Failed to load data" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(GLenum TextureNumber, std::string path)
{
	glGenTextures(1, &ID);
	texture_number = TextureNumber;
	texPath = path;

	//std::cout << "Texture Number & Static Texture Number: " << texture_number << " " << current_texture_number << std::endl;

	glActiveTexture(texture_number);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* image_data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	//if loaded successfully
	if (image_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (nrChannels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(image_data);
	}
	else
	{
		std::cout << "Failed to load data" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind()
{
	glActiveTexture(texture_number);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
