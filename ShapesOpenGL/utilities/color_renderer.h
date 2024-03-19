#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class ColorRenderer
{
public:
    // Constructor (inits shaders/shapes)
    ColorRenderer(Shader& shader);
    // Destructor
    ~ColorRenderer();
    // Renders a defined quad textured with given Color
    void DrawColor(glm::vec2 position
        , glm::vec2 size = glm::vec2(10.0f, 10.0f)
        , float rotate = 0.0f
        , glm::vec3 color = glm::vec3(0.0f));
    //void DrawColorFromLeft(glm::vec2 position, glm::vec3 color = glm::vec3(0.0f));
    //void DrawColorFromRight();
private:
    // Render state
    Shader       shader;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};
