#pragma once
#include "glm/glm.hpp"

class Collider
{
public:
	virtual glm::vec3 FindFurthestPoint(glm::vec3 direction) const = 0;
};

