#pragma once
#include<glm/glm.hpp>
#include "utilities/VAO.h"
#include "utilities/shader.h"
#include "Shape.h"

class Ellipsoid : public Shape
{
public:
	Ellipsoid(Transform transform
		, glm::vec4 color = glm::vec4(0.0f));

	Ellipsoid(glm::vec3 position
		, glm::vec3 size
		, glm::vec3 rotation
		, glm::vec4 color = glm::vec4(0.0f));

	void Init() override;

	//glm::vec3 Size;
	//glm::vec3 Position;
	//glm::vec3 Rotation;
	//glm::vec4 Color;

	//bool isDrawn = true;
	//bool isWired = true;

	//VAO* vao;

	

	void Draw(Shader& shader) const override;
};

