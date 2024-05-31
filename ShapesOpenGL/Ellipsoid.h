#pragma once
// #include <glm/gtx/quaternion.hpp>
#include<glm/glm.hpp>

#include "utilities/VAO.h"
#include "utilities/shader.h"
#include "MeshCollider.h"
#include "Shape.h"

struct EllipsoidStruct
{
	glm::vec4 position;
	glm::vec4 size;
};

class Ellipsoid : public Shape
{
public:
	Ellipsoid(Transform transform
		, glm::vec4 color = glm::vec4(0.0f));

	Ellipsoid(glm::vec3 position
		, glm::vec3 size
		, glm::quat rotation
		, glm::vec4 color = glm::vec4(0.0f));

	void Init() override;

	std::unique_ptr<MeshCollider> collider;

	//glm::vec3 Size;
	//glm::vec3 Position;
	//glm::vec3 Rotation;
	//glm::vec4 Color;

	//bool isDrawn = true;
	//bool isWired = true;

	//VAO* vao;

	

	void Draw(Shader& shader) const override;
};

