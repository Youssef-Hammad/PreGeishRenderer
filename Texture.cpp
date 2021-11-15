#include "Texture.h"

Texture::Texture(std::string path)
{
	glGenTextures(1, &ID);
	texture_number = current_texture_number;
	current_texture_number += 1;

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
	}
	else
	{
		std::cout << "Failed to load data" << std::endl;
	}
	stbi_image_free(image_data);

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
