#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO VBO
    , GLuint index
    , GLint size
    , GLenum type
    , GLboolean normalized
    , GLsizei stride
    , const void * pointer)
{
    if (!ID)
        Generate();

    VBO.Bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
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

