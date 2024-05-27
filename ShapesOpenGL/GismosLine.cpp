#include "GismosLine.h"

CGismosLine::CGismosLine(glm::vec3 direction) : Cuboid(glm::vec3(0.0f))
{
	if (direction == glm::vec3(1.0f, 0.0f, 0.0f))
	{
		transform.Position = glm::vec3(GISMOS_LINE_LENGTH / 2.0f, 0, 0);
		transform.Size = glm::vec3(GISMOS_LINE_LENGTH, GISMOS_LINE_SIZE, GISMOS_LINE_SIZE);
		transform.Rotation = glm::vec3(0.0f);
		color = RED;
	}
	else if (direction == glm::vec3(0.0f, 1.0f, 0.0f))
	{
		transform.Position = glm::vec3(0, 0.0f, 0);
		transform.Size = glm::vec3(GISMOS_LINE_SIZE, GISMOS_LINE_LENGTH* 2.0f, GISMOS_LINE_SIZE);
		transform.Rotation = glm::vec3(0.0f);
		color = GREEN;
	}
	else if (direction == glm::vec3(0.0f, 0.0f, 1.0f))
	{
		transform.Position = glm::vec3(0, 0, GISMOS_LINE_LENGTH / 2.0f);
		transform.Size = glm::vec3(GISMOS_LINE_SIZE, GISMOS_LINE_SIZE, GISMOS_LINE_LENGTH);
		transform.Rotation = glm::vec3(0.0f);
		color = BLUE;
	}
	isWired = false;
}
