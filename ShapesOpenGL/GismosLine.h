#pragma once
#include "Cuboid.h"

const float GISMOS_LINE_SIZE = 0.07f;
const float GISMOS_LINE_LENGTH = 50.0f;

const glm::vec4 RED = glm::vec4(-0.5f + 1.0f, -0.5f, -0.5f, 1.0f);
const glm::vec4 GREEN = glm::vec4(-0.5f, -0.5f + 1.0f, -0.5f, 1.0f);
const glm::vec4 BLUE = glm::vec4(-0.5f, -0.5f, -0.5f + 1.0f, 1.0f);

class CGismosLine :
    public Cuboid
{
public:
	CGismosLine(glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f));
};

