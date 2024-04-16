#pragma once

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
	GLuint ID = 0;
	VAO();

	void LinkVBO(VBO VBO
		, GLuint index
		, GLint size
		, GLenum type
		, GLboolean normalized
		, GLsizei stride
		, const void* pointer);

	void Generate();
	void Bind();
	void Unbind();
	void Delete();
};

