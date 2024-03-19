#include "Shape.h"

Shape::Shape(Transform transform, glm::vec4 color) 
	: transform(transform), color(color)
{
	// Init();
}

Shape::Shape(glm::vec3 position
	, glm::vec3 size
	, glm::quat rotation
	, glm::vec4 color)
	: transform(Transform(position, size, rotation)), color(color)
{
	// Init();
}
