#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

struct Transform
{
	glm::vec3 Position;
	glm::vec3 Size;
	glm::quat Quaternion;

	glm::vec3 Rotation;

	Transform(glm::vec3 position, glm::vec3 size, glm::quat quaternion)
		: Position(position), Size(size), Quaternion(quaternion), Rotation(glm::vec3(0.0f)) { }
};
