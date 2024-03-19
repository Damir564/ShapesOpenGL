#include "GJK.h"

glm::vec3 Support(const Collider& colliderA, const Collider& colliderB, glm::vec3 direction)
{
	return colliderA.FindFurthestPoint(direction)
		- colliderB.FindFurthestPoint(-direction);
}

bool SameDirection(const glm::vec3& direction, const glm::vec3& ao)
{
	return glm::dot(direction, ao) > 0;
}

bool Line(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];

	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;

	if (SameDirection(ab, ao)) {
		direction = cross(cross(ab, ao), ab);
	}

	else {
		points = { a };
		direction = ao;
	}

	return false;
}

bool Triangle(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = cross(ab, ac);

	if (SameDirection(cross(abc, ac), ao)) {
		if (SameDirection(ac, ao)) {
			points = { a, c };
			direction = cross(cross(ac, ao), ac);
		}

		else {
			return Line(points = { a, b }, direction);
		}
	}

	else {
		if (SameDirection(cross(ab, abc), ao)) {
			return Line(points = { a, b }, direction);
		}

		else {
			if (SameDirection(abc, ao)) {
				direction = abc;
			}

			else {
				points = { a, c, b };
				direction = -abc;
			}
		}
	}

	return false;
}

bool Tetrahedron(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];
	glm::vec3 d = points[3];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ad = d - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = cross(ab, ac);
	glm::vec3 acd = cross(ac, ad);
	glm::vec3 adb = cross(ad, ab);

	if (SameDirection(abc, ao)) {
		return Triangle(points = { a, b, c }, direction);
	}

	if (SameDirection(acd, ao)) {
		return Triangle(points = { a, c, d }, direction);
	}

	if (SameDirection(adb, ao)) {
		return Triangle(points = { a, d, b }, direction);
	}

	return true;
}

bool NextSimplex(Simplex& points, glm::vec3& direction)
{
	switch (points.size()) {
	case 2: return Line(points, direction);
	case 3: return Triangle(points, direction);
	case 4: return Tetrahedron(points, direction);
	}

	// never should be here
	return false;
}

bool GJK(const Collider& colliderA
	, const Collider& colliderB)
{
	// Get initial support point in any direction
	glm::vec3 support = Support(colliderA, colliderB, glm::vec3(1, 0, 0));
	// Simplex is an array of points, max count is 4
	Simplex points;
	points.push_front(support);

	// New direction is towards the origin
	glm::vec3 direction = -support;
	while (true) {
		support = Support(colliderA, colliderB, direction);

		if (dot(support, direction) <= 0) {
			return false; // no collision
		}

		points.push_front(support);
		if (NextSimplex(points, direction)) {
			return true;
		}
	}
}
