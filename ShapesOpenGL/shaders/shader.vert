#version 460 core
layout (location = 0) in vec3 vertex; // <vec2 position, vec2 texCoords>

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform vec4 color;

out vec4 fcolor;
out float ftime;

void main(void)
{
    // gl_PointSize = 6.0f;
    gl_Position = projection * view * model * vec4(vertex, 1.0);
    fcolor = color; 
    ftime = time;
}