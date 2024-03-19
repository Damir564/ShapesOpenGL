#include "Ellipsoid.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "utilities/EBO.h"
#include <iostream>

const int stacks = 20;
const int slices = 20;


Ellipsoid::Ellipsoid(Transform transform, glm::vec4 color)
    : Shape(transform, color)
{
    Init();
}

Ellipsoid::Ellipsoid(glm::vec3 position
    , glm::vec3 size
    , glm::vec3 rotation
    , glm::vec4 color)
    : Shape(position, size, rotation, color)
{
    Init();
}

void Ellipsoid::Init()
{
    const float M_PI = 3.14159f;
    const float _2pi = 2.0f * M_PI;

    // input values
    float a = 0.5f;
    float b = 0.5f;
    float c = 0.5f;
    //***

    std::vector<glm::vec3> ellipsoidVertices;
    // std::vector<glm::vec3> ellipsoidNormals;
    // std::vector<glm::vec2> ellipsoidTextureCoords;

    for (int i = 0; i <= stacks; ++i)
    {
        // V texture coordinate
        float V = i / (float)stacks;
        float phi = V * M_PI - M_PI / 2.0f;
        for (int j = 0; j <= slices; ++j)
        {

            // U texture coordinate
            float U = j / (float)slices;
            float theta = U * _2pi;

            float X = a * cos(phi) * cos(theta);
            float Y = b * cos(phi) * sin(theta);
            float Z = c * sin(phi);

            ellipsoidVertices.push_back(glm::vec3(X, Y, Z));
            // ellipsoidNormals.push_back(glm::vec3(X, Y, Z));
            // ellipsoidTextureCoords.push_back(glm::vec2(U, V));
        }
    }

    // Now generate the index buffer
    std::vector<GLuint> ellipsoidIndicies;

    int noPerSlice = slices + 1;
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {

            int start_i = (i * noPerSlice) + j;

            ellipsoidIndicies.push_back(start_i);
            ellipsoidIndicies.push_back(start_i + noPerSlice + 1);
            ellipsoidIndicies.push_back(start_i + noPerSlice);

            ellipsoidIndicies.push_back(start_i + noPerSlice + 1);
            ellipsoidIndicies.push_back(start_i);
            ellipsoidIndicies.push_back(start_i + 1);
        }
    }

    vao = new VAO();
    vao->Bind();

    VBO vbo((GLfloat*)ellipsoidVertices.data(), ellipsoidVertices.size() * sizeof(glm::vec3));
    vao->LinkVBO(vbo, 0);

    EBO ebo(ellipsoidIndicies.data(), ellipsoidIndicies.size() * sizeof(GLuint));

    vao->Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Ellipsoid::Draw(Shader& shader) const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.Position); 

    model = glm::scale(model, transform.Size);

    shader.SetMatrix4("model", model);
    shader.SetVector4f("color", color);

    vao->Bind();
    glDrawElements(GL_TRIANGLES, (slices * stacks + slices) * 3 * 2, GL_UNSIGNED_INT, nullptr);
    vao->Unbind();
}
