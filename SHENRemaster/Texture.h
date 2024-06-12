#pragma once

#include <glad/glad.h>

#include "Shader.h"

class Texture {
public:
	GLuint ID;
	const char* type;
	GLuint unit;
	
	Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

	void AssignUnit(Shader& shader, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();

	void Free();
};

