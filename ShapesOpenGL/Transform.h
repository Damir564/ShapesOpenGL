#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

struct Transform
{
	glm::vec3 Position;
	glm::vec3 Size;
	glm::quat Rotation;

	Transform(glm::vec3 position, glm::vec3 size, glm::quat rotation)
		: Position(position), Size(size), Rotation(rotation) { }
};
