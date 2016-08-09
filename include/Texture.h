#pragma once

//std includes
#include <string.h>

//GL includes
#include <GL/glew.h>
#include <SOIL.h>

class Texture 
{
public:
	int width, height;
	GLuint texture;

	unsigned char* image;

	Texture(std::string imagePath)
	{
		image = SOIL_load_image(imagePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Use(GLint location, GLint textureUnit)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(location, textureUnit);
	}

};