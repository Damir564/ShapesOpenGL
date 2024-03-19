#version 460 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
// uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    TexCoords = vertex.zw;
    // gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
}