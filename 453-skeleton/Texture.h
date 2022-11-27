#pragma once

#include "GLHandles.h"
#include <GL/glew.h>
#include <string>
#include <stb/stb_image.h>
#include <iostream>

#include <glm/glm.hpp>


class Texture {
public:
	Texture(std::string path, GLint interpolation);

	// Because we're using the TextureHandle to do RAII for the texture for us
	// and our other types are trivial or provide their own RAII
	// we don't have to provide any specialized functions here. Rule of zero
	//
	// https://en.cppreference.com/w/cpp/language/rule_of_three
	// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero

	// Public interface
	std::string getPath() const { return path; }
	GLenum getInterpolation() const { return interpolation; }

	// Although uint (i.e. uvec2) might make more sense here, went with int (i.e. ivec2) under
	// the assumption that most students will want to work with ints, not uints, in main.cpp
	glm::ivec2 getDimensions() const { return glm::uvec2(width, height); }

	void bind() { glBindTexture(GL_TEXTURE_2D, textureID); }
	void unbind() { glBindTexture(GL_TEXTURE_2D, textureID); }

	static unsigned int loadTexture(const char* path) {

		//Load Texture
		int width = 0;
		int height = 0;
		int channels = 3;
		unsigned char* image = stbi_load(path, &width, &height,&channels, 0);
		// Generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);


		if (image)
		{
			GLenum format = GL_RGB;
			if (channels == 4) {
				GLenum format = GL_RGBA;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
		}
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(image);

		return texture;
	}


private:
	TextureHandle textureID;
	std::string path;
	GLint interpolation;


	// Although uint might make more sense here, went with int under the assumption
	// that most students will want to work with ints, not uints, in main.cpp
	int width;
	int height;



};
