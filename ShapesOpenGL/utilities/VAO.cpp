#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO VBO, GLuint layout)
{
    if (!ID)
        Generate();

    VBO.Bind();
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

void VAO::Generate()
{
    // glGenVertexArrays(1, &ID);
}

void VAO::Bind()
{
    if (!ID)
        Generate();

    glBindVertexArray(ID);
}

void VAO::Unbind()
{
    if (!ID)
        Generate();

    glBindVertexArray(0);
}

void VAO::Delete()
{
    if (!ID)
        Generate();

    glDeleteVertexArrays(1, &ID);
}

