#include "Cuboid.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "utilities/EBO.h"
#include <iostream>

GLfloat cuboidVertices[] = {
    -0.5, -0.5, -0.5,
    -0.5, -0.5, 0.5,
    -0.5, 0.5,  -0.5,
    -0.5, 0.5,   0.5,
    0.5,  -0.5, -0.5,
    0.5,  -0.5, 0.5,
    0.5,  0.5,  -0.5,
    0.5,  0.5,  0.5,
};

GLuint cuboidIndices[] = {
    0, 1, 2, 1, 3, 2,
    0, 1, 4, 1, 5, 4,
    2, 3, 6, 3, 7, 6,
    4, 6, 5, 5, 6, 7,
    6, 0, 2, 6, 4, 0,
    7, 1, 5, 7, 3, 1,
};

Cuboid::Cuboid(Transform transform, glm::vec4 color)
    : Shape(transform, color)
{
    Init();
}

Cuboid::Cuboid(glm::vec3 position
    , glm::vec3 size
    , glm::quat rotation
    , glm::vec4 color)
	:  Shape(position, size, rotation, color)// transform(position, size, rotation), color(color)
{
    Init();
}

void Cuboid::Init()
{
    vao = new VAO();
    vao->Bind();

    // Generate & bind VBO
    VBO vbo(cuboidVertices, sizeof(cuboidVertices));
    // Link VBO to the VAO
    vao->LinkVBO(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Generate & bind EBO
    EBO ebo(cuboidIndices, sizeof(cuboidIndices));

    // Unbind
    vao->Unbind();
    vbo.Unbind();
    ebo.Unbind();

    // Physics
    collider = std::make_unique<MeshCollider>(MeshCollider(std::vector<float>(cuboidVertices
                    , cuboidVertices + sizeof(cuboidVertices) / sizeof(cuboidVertices[0]))));

    //for (const auto& vertex : static_cast<MeshCollider*>(collider.get())->m_vertices)
    //{
    //    std::cout << "x = " << vertex.x << "; y = " << vertex.y << "; z = " << vertex.z << '\n';
    //}
}

void Cuboid::Draw(Shader& shader) const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.Position);  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)   
    // model = glm::rotate(model, transform.Rotation);
    model = glm::rotate(model, glm::radians(transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, transform.Size); // last scale
    //collider->ApplyModelMat4(model);
    collider->ApplyTransform(transform);

    shader.SetMatrix4("model", model);
    shader.SetVector4f("color", color);
    // shader.SetVector3f("color", color);

    vao->Bind();
    glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_INT, nullptr);
    vao->Unbind();
}

