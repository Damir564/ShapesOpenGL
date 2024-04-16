#pragma once
#include "Collider.h"
#include "glm/glm.hpp"
#include "Transform.h"
#include <vector>
class MeshCollider : public Collider
{
private:
	std::vector<glm::vec3> m_normalized_vertices;
public:
	// std::vector<glm::vec3> m_normalized_vertices;
	std::vector<glm::vec3> vertices;
	MeshCollider(const std::vector<float> & rawVertices) 
	{
		if (rawVertices.size() % 3 != 0)
			return;

		for (int i = 0; i != rawVertices.size(); i += 3)
		{
			m_normalized_vertices.push_back(glm::vec3(rawVertices[i]
				, rawVertices[i + 1]
				, rawVertices[i + 2]));
		}
		vertices = std::vector<glm::vec3>(rawVertices.size() / 3);
	}

	MeshCollider(const std::vector<glm::vec3>& rawVertices)
	{
		m_normalized_vertices = rawVertices;
		vertices = std::vector<glm::vec3>(rawVertices.size());
	}

	void ApplyTransform(const Transform& transform)
	{
		for (int i = 0; i != vertices.size(); ++i)
		{
			vertices[i] = m_normalized_vertices[i] * transform.Size + transform.Position;
		}
	}

	void ApplyModelMat4(const glm::mat4& model)
	{
		for (int i = 0; i != vertices.size(); ++i)
		{
			vertices[i] = model * glm::vec4(vertices[i], 1.0f);
		}
	}

	glm::vec3 FindFurthestPoint(glm::vec3 direction) const override
	{
		glm::vec3  maxPoint ;
		float maxDistance = -FLT_MAX;

		for (glm::vec3 vertex : vertices) {
			float distance = glm::dot(vertex, direction);
			if (distance > maxDistance) {
				maxDistance = distance;
				maxPoint = vertex;
			}
		}

		return maxPoint;
	}
};

