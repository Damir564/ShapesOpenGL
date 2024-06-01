#pragma once
#include<glm/glm.hpp>
#include "Transform.h"
#include "utilities/VAO.h"
#include "utilities/shader.h"
#include "Shape.h"
#include "MeshCollider.h"

// class VAO;

struct CuboidStruct
{
	glm::vec4 position;
	glm::vec4 size;
};

class Cuboid : public Shape
{
public:
	//glm::vec3 Size;
	//glm::vec3 Position;
	//glm::vec3 Rotation;
	Cuboid(Transform transform
		, glm::vec4 color = glm::vec4(0.0f));

	Cuboid(glm::vec3 position
		, glm::vec3 size = glm::vec3(1.0f)
		, glm::quat rotation = glm::quat()
		, glm::vec4 color = glm::vec4(0.0f));

	std::unique_ptr<MeshCollider> collider;

	void Init() override;

	//Transform transform;
	//glm::vec4 color;

	//bool isDrawn = true;
	//bool isWired = true;

	VAO* vao;

	void Draw(Shader& shader) const override;
};
