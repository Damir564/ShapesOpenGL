#pragma once
#include <glm/glm.hpp>
#include "Collider.h"
#include "Simplex.h"

//struct GJK
//{
//public:
//	GJK() {};

glm::vec3 Support(const Collider& colliderA, const Collider& colliderB, glm::vec3 direction);
bool SameDirection(const glm::vec3& direction, const glm::vec3& ao);
bool Line(Simplex& points, glm::vec3& direction);
bool Triangle(Simplex& points, glm::vec3& direction);
bool Tetrahedron(Simplex& points, glm::vec3& direction);
bool NextSimplex(Simplex& points, glm::vec3& direction);
bool GJK(const Collider& colliderA
	, const Collider& colliderB);






//};

