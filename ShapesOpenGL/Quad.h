#pragma once
#include "utilities/VAO.h"
#include "utilities/shader.h"

class Quad 
{
public:
	Quad();

	//Transform transform;
	//glm::vec4 color;

	//bool isDrawn = true;
	//bool isWired = true;

	VAO* vao;

	void Draw(Shader& shader, GLuint fbo) const;
};

