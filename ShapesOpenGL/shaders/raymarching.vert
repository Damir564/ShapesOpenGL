#version 460 core
layout (location = 0) in vec2 inVertex;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;


void main(void)
{
    gl_Position = vec4(inVertex, 0.0, 1.0);
    texCoords = inTexCoords;
}