#version 460 core
layout (location = 0) in vec2 vertex; // <vec2 position>
// layout (location = 1) in vec3 color;

// out vec3 ourColor;

uniform mat4 model;
// uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    // ourColor = color;
    gl_Position = projection * model * vec4(vertex, 0.0, 1.0);
}