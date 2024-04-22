#version 460 core
layout (location = 0) in vec3 vertex; // <vec2 position, vec2 texCoords>

//layout (std140) uniform Matrices
//{
//    mat4 projection;
//    // mat4 view;
//};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
// uniform vec2 resolution;

// out vec4 fcolor;
out float ftime;
// out vec2 fresolution;

void main(void)
{
    // gl_PointSize = 6.0f;
    gl_Position = projection * view * model * vec4(vertex, 1.0);
    // fcolor = color; 
    ftime = time;
    // fresolution = resolution;
}