#include "Quad.h"
#include "utilities/EBO.h"

GLfloat quadVertices[] = {
    -1.0, -1.0, 0.0, 0.0,
    1.0, -1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0,
    -1.0, 1.0, 0.0, 1.0,
};

GLuint quadIndices[] = {
    0, 1, 2, 0, 2, 3
};

Quad::Quad() {
    vao = new VAO();
    vao->Bind();

    // Generate & bind VBO
    VBO vbo(quadVertices, sizeof(quadVertices));
    // Link VBO to the VAO
    vao->LinkVBO(vbo, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    vao->LinkVBO(vbo, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    // Generate & bind EBO
    EBO ebo(quadIndices, sizeof(quadIndices));

    // Unbind
    vao->Unbind();
    vbo.Unbind();
    ebo.Unbind();
}


void Quad::Draw(Shader& shader, GLuint fbo) const
{
    vao->Bind();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, fbo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    vao->Unbind();
}
