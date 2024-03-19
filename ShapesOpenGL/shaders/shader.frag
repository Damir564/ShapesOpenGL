#version 460 core

vec3 color1 = vec3(0.0, 0.0, 0.0);
vec3 color2 = vec3(0.02, 1.0, 0.02);

in vec4 fcolor;
in float ftime;

float near = 0.1; 
float far  = 100.0; 

out vec4 FragColor;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{

    float depth = LinearizeDepth(gl_FragCoord.z) / far ;
    FragColor = vec4(vec3(depth + fcolor.xyz), fcolor.w);
    //FragColor = vec4(color2, 1.0);
}