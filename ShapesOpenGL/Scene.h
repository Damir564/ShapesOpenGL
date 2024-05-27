#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "utilities/VAO.h"
#include "Cuboid.h"
#include "Ellipsoid.h"
#include "GismosLine.h"
#include "Quad.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// class VAO;

class Scene
{
public:
    unsigned int Width, Height;
    // KeyBoard
    bool Keys[1024];
    bool KeysProcessed[1024];

    
    // Mouse
    bool MouseButtons[8];
    bool MouseButtonsProcessed[8];
    void SetMouseWheelYOffset(double yOffset)
    {
        _mouseWheelYOffset = (float)yOffset;
    };
    double mousePosXOffset;
    double mouseFirstXPos = 0.0;
    double mouseFirstYPos = 0.0;
    double mouseXPos = 0.0;
    double mouseYPos = 0.0;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    // Camera
    float cameraYaw = -90.0f;
    float cameraPitch = 0.0f;

    // VAO* vao1Ptr;
    std::vector<VAO> vaoVector;

    GLuint FBO;
    GLuint framebufferTexture;
    GLuint RBO;

    GLuint ellipsoidVao;
    GLuint ellipsoidVbo[2];

    std::vector<std::unique_ptr<CGismosLine>> GismosLines;
    std::vector<std::unique_ptr<Cuboid>> GismosMarks;
    std::vector<std::unique_ptr<Cuboid>> Cuboids;
    std::vector<std::unique_ptr<Ellipsoid>> Ellipsoids;
    std::unique_ptr<Quad> ScreenQuad;

	Scene(unsigned int width, unsigned int height);
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void DoCollisions();
    void Render();
    void UI();
    void Dispose();
private:
    // Scene objects
    //bool _drawCuboid = true;
    //bool _drawEllipsoid = true;
    //bool _wiredEllipsoid = false;

    bool _firstMouse = true;
    float _mouseWheelYOffset = 0.0f;

    bool _raymarching = false;
    
    //float _lastMouseWheelYOffset = 0.0f;
    //float _deltaMouseWheelYOffset = 0.0f;
};
