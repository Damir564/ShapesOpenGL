#pragma once
#include "Transform.h"
#include<glm/glm.hpp>
#include<memory>
#include "utilities/VAO.h"
#include "utilities/shader.h"
#include "Collider.h"
class Shape
{
public:
	Shape(Transform transform
		, glm::vec4 color = glm::vec4(0.0f));

	Shape(glm::vec3 position
		, glm::vec3 size = glm::vec3(1.0f)
		, glm::quat rotation = glm::quat()
		, glm::vec4 color = glm::vec4(0.0f));


	virtual void Init() = 0;

	// std::unique_ptr<Collider> collider;

	Transform transform;
	glm::vec4 color;

	bool isDrawn = true;
	bool isWired = true;

	VAO* vao;

	virtual void Draw(Shader& shader) const = 0;
};

