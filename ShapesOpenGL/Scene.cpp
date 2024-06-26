#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include "Scene.h"
// #include "GJK.h"
#include "glm/glm.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <format>
#include "utilities/resource_manager.h"
#include "utilities/sprite_renderer.h"
// #include "utilities/VAO.h"
#include "utilities/VBO.h"
#include "utilities/EBO.h"
#include "Simplex.h"
#include "GJK.h"




glm::vec3 cameraPos = glm::vec3(5.0f, 15.0f, 60.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


Scene::Scene(unsigned int width, unsigned int height) : Width(width), Height(height), Keys(), KeysProcessed(), Cuboids() {}



void Scene::Init()
{
    ResourceManager::LoadShader("shaders/shader.vert", "shaders/shader.frag", nullptr, "shader");
    ResourceManager::LoadShader("shaders/raymarching.vert", "shaders/raymarching.frag", nullptr, "raymarching");

    //GLuint uniformBlockIndex = 
    //    glGetUniformBlockIndex(ResourceManager::GetShader("shader").ID, "Matrices");
    //
    //glUniformBlockBinding(ResourceManager::GetShader("shader").ID
    //    , uniformBlockIndex
    //    , 0);

    GismosLines.push_back(std::make_unique<CGismosLine>(CGismosLine(glm::vec3(1.0f, 0.0f, 0.0f))));
    GismosLines.push_back(std::make_unique<CGismosLine>(CGismosLine(glm::vec3(0.0f, 1.0f, 0.0f))));
    GismosLines.push_back(std::make_unique<CGismosLine>(CGismosLine(glm::vec3(0.0f, 0.0f, 1.0f))));

    for (int i = 0; i != 100 / 2; ++i)
    {
        GismosMarks.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(i * 2.0f - 50.0f, 0.0f, 0.0f)
            , glm::vec3(0.04f, 0.04f, 100.0f)
            , glm::vec3(0.0f, 0.0f, 0.0f)
            , glm::vec4(0.0f, 0.0f, 0.0f, 0.2f))));
        GismosMarks.at(GismosMarks.size() - 1)->isWired = false;
        GismosMarks.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(0.0f, 0.0f, i * 2.0f - 50.0f)
            , glm::vec3(100.0f, 0.04f, 0.04f)
            , glm::vec3(0.0f, 0.0f, 0.0f)
            , glm::vec4(0.0f, 0.0f, 0.0f, 0.2f))));
        GismosMarks.at(GismosMarks.size() - 1)->isWired = false;
        GismosMarks.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(0.0f, i * 2.0f - 50.0f, 0.0f)
            , glm::vec3(0.08f, 0.06f, 0.08f)
            , glm::vec3(0.0f, 0.0f, 0.0f)
            , glm::vec4(0.0f, 0.0f, 0.0f, 0.2f))));
        GismosMarks.at(GismosMarks.size() - 1)->isWired = false;
        //Ellipsoids.push_back(std::make_unique<Ellipsoid>(Ellipsoid(glm::vec3(i * 2.0f, 0.0f, 0.0f)
        //    , glm::vec3(0.2f, 0.2f, 0.2f)
        //    , glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
        //    , glm::vec4(-0.5f, -0.5f, -0.5f, 2.0f))));
        //Ellipsoids.at(Ellipsoids.size() - 1)->isWired = false;
    }

    

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        static_cast<float>(this->Width) / static_cast<float>(this->Height),
        0.1f, 100.0f);
     ResourceManager::GetShader("shader").Use();
    
     ResourceManager::GetShader("shader").SetMatrix4("projection", projection);
     

    ResourceManager::GetShader("raymarching").Use();
    ResourceManager::GetShader("raymarching").SetVector2f("uResolution", Width, Height);
    // ResourceManager::GetShader("raymarching").SetFloat("uTime", glfwGetTime());

    Ellipsoids.push_back(std::make_unique<Ellipsoid>(Ellipsoid(glm::vec3(0.0f, 0.0f, 0.0f)
        , glm::vec3(4.0f, 2.0f, 2.0f)
        // , glm::quat(0.7071f, 0.0f, 0.7071f, 0.0f)
        , glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
        , glm::vec4(0.0f, 0.0f, 0.0f, 0.2f))));

    Ellipsoids.push_back(std::make_unique<Ellipsoid>(Ellipsoid(glm::vec3(1.0f, 2.0f, 0.0f)
        , glm::vec3(4.0f, 5.0f, 2.0f)
        // , glm::quat(0.7071f, 0.0f, 0.7071f, 0.0f)
        , glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
        , glm::vec4(0.0f, 0.0f, 0.0f, 0.2f))));

    //Ellipsoids.push_back(std::make_unique<Ellipsoid>(Ellipsoid(glm::vec3(-2.0f, 0.0f, 0.0f)
    //    , glm::vec3(1.0f, 2.0f, 10.0f)
    //    // , glm::quat(0.7071f, 0.0f, 0.7071f, 0.0f)
    //    , glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
    //    , glm::vec4(0.0f, 0.0f, 0.0f, 0.2f))));

    Cuboids.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(10.0f, 2.0f, 2.0f)
        , glm::vec3(2.0f, 5.0f, 10.0f)
        , glm::vec3(0.0f, 0.0f, 0.0f)
        , glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))));

    Cuboids.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(5.0f, 2.0f, 2.0f)
        , glm::vec3(2.0f, 5.0f, 10.0f)
        , glm::vec3(0.0f, 0.0f, 0.0f)
        , glm::vec4(0.0f, 1.0f, 1.0f, 1.0f))));

    uniformBlockIndexEllipsoids = glGetUniformBlockIndex(ResourceManager::GetShader("raymarching").ID, "EllipsoidBlock");
    uniformBlockIndexCuboids = glGetUniformBlockIndex(ResourceManager::GetShader("raymarching").ID, "CuboidBlock");

    //glUniformBlockBinding(ResourceManager::GetShader("raymarching").ID, uniformBlockIndexEllipsoids, 0);
    //glUniformBlockBinding(ResourceManager::GetShader("raymarching").ID, uniformBlockIndexCuboids, 0);

    //for (int i = 0; i != 100; ++i)
    //{
    //    EllipsoidBlock[i] = EllipsoidStruct();
    //}

    glGenBuffers(1, &uboEllipsoids);
    glBindBuffer(GL_UNIFORM_BUFFER, uboEllipsoids);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(EllipsoidStruct) * 100, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboEllipsoids);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboEllipsoids, 0, sizeof(EllipsoidStruct) * 100);

    glGenBuffers(1, &uboCuboids);
    glBindBuffer(GL_UNIFORM_BUFFER, uboCuboids);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CuboidsTransform), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboCuboids);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);


    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "framebuffer error" << fboStatus << std::endl;

    ScreenQuad = std::make_unique<Quad>(Quad());
}

void Scene::ProcessInput(float dt)
{
    float cameraSpeed = static_cast<float>(4.0f * dt);
    float cameraSpeedMultiplier = 1.0f;
    float cameraZoomMultiplier = 100.0f;
    if (Keys[GLFW_KEY_LEFT_SHIFT])
        cameraSpeedMultiplier = 4.0f;
    else if (Keys[GLFW_KEY_LEFT_CONTROL])
        cameraSpeed /= 2;
    if (Keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraUp * cameraSpeedMultiplier;
    if (Keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraUp * cameraSpeedMultiplier;
    if (Keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * cameraSpeedMultiplier;
    if (Keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * cameraSpeedMultiplier;
    if (Keys[GLFW_KEY_E])
        cameraPos += cameraSpeed * cameraFront * cameraSpeedMultiplier;
    if (Keys[GLFW_KEY_C])
        cameraPos -= cameraSpeed * cameraFront * cameraSpeedMultiplier;


    // _deltaMouseWheelYOffset = _mouseWheelYOffset - _lastMouseWheelYOffset;
    // std::cout << _deltaMouseWheelYOffset << "; _deltaMouseWheelYOffset" << std::endl;
    if (MouseButtons[GLFW_MOUSE_BUTTON_LEFT])
    {
        if (!MouseButtonsProcessed[GLFW_MOUSE_BUTTON_LEFT])
        {
            mouseFirstXPos = mouseXPos;
            mouseFirstYPos = mouseYPos;
            MouseButtonsProcessed[GLFW_MOUSE_BUTTON_LEFT] = true;
        }
        float xoffset = mouseFirstXPos - mouseXPos;
        float yoffset = mouseYPos - mouseFirstYPos;
        // std::cout << xoffset << "; xoffset" << std::endl;
        mouseFirstXPos = mouseXPos;
        mouseFirstYPos = mouseYPos;
        cameraPos += cameraSpeed * cameraUp * yoffset * 8.0f;//* (float)Height;
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * xoffset * 8.0f;
        // std::cout << yoffset << "; yoffset" << std::endl;
    }
    else if (MouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
    {
        if (!MouseButtonsProcessed[GLFW_MOUSE_BUTTON_RIGHT])
        {
            mouseFirstXPos = mouseXPos;
            mouseFirstYPos = mouseYPos;
            MouseButtonsProcessed[GLFW_MOUSE_BUTTON_RIGHT] = true;
        }
        float xoffset = mouseXPos - mouseFirstXPos;
        float yoffset = mouseFirstYPos - mouseYPos;
        mouseFirstXPos = mouseXPos;
        mouseFirstYPos = mouseYPos;
        // std::cout << xoffset << "; xoffset" << std::endl;
        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        cameraYaw += xoffset;
        cameraPitch += yoffset;

        if (cameraPitch > 89.0f)
            cameraPitch = 89.0f;
        if (cameraPitch < -89.0f)
            cameraPitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        direction.y = sin(glm::radians(cameraPitch));
        direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = glm::normalize(direction);
        std::cout << "X: " << cameraFront.x << " Y: " << cameraFront.y << " Z: " << cameraFront.z << "\n";

        
    }
    if (_mouseWheelYOffset)
    {
        cameraPos += _mouseWheelYOffset * cameraSpeed * cameraZoomMultiplier * cameraFront;
    }
    _mouseWheelYOffset = 0.0f;
}

void Scene::Update(float dt)
{
}

void Scene::DoCollisions()
{
    for (const auto& a : Cuboids)
    {
        for (const auto& b : Cuboids)
        {
            if (a == b)
                continue;


        }
        for (const auto& b : Ellipsoids)
        {
            // if (GJK(*a->collider, *b->collider))
            //    std::cout << "Yessss!" << '\n';
            //else
            //    std::cout << "____________No:(" << '\n';
        }
    }
}

void Scene::Render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    ResourceManager::GetShader("shader").Use();
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    ResourceManager::GetShader("shader").SetMatrix4("view", view); // make sure to initialize matrix to identity matrix first

    glLineWidth(1.0f);
    int i = 0;
    for (std::unique_ptr<Ellipsoid> const& ellipsoid : Ellipsoids)
    {
        if (ellipsoid->isWired)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (ellipsoid->isDrawn)
            ellipsoid->Draw(ResourceManager::GetShader("shader").Use());       

        // EllipsoidStruct ellipsoidStructs[1];
        EllipsoidStruct temp = { .position = glm::vec4(ellipsoid->transform.Position, 1.0f)
            , .size = glm::vec4(ellipsoid->transform.Size, 1.0f) };
        EllipsoidsTransform[i] = temp;
        i++;
    } 
    int numEllipsoids = i;
    ResourceManager::GetShader("raymarching").Use();
    ResourceManager::GetShader("raymarching").SetInteger("u_iCurrentEllipsoidsNum", Ellipsoids.size());
    glBindBuffer(GL_UNIFORM_BUFFER, uboEllipsoids);
    //for (int i = 0; i != numEllipsoids; ++i)
    //{
    //    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * i * 2, sizeof(glm::vec4), &EllipsoidsTransform[i].position);
    //    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * i * 2 + sizeof(glm::vec4), sizeof(glm::vec4), &EllipsoidsTransform[i].size);
    //    // std::cout << EllipsoidsTransform[i].position.x << " " << EllipsoidsTransform[i].position.y << " " << EllipsoidsTransform[i].position.z << "value pos\n";
    //    // std::cout << EllipsoidsTransform[i].size.x << " " << EllipsoidsTransform[i].size.y << " " << EllipsoidsTransform[i].size.z << "value size\n\n";
    //}
    glBufferData(GL_UNIFORM_BUFFER, sizeof(EllipsoidStruct) * 100, EllipsoidsTransform, GL_DYNAMIC_DRAW);

    // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(EllipsoidStruct) * 100, EllipsoidsTransform);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // std::cout << "end\n\n\n" << std::endl;
    //glBindBuffer(GL_UNIFORM_BUFFER, uboEllipsoids);
    //glBufferData(GL_UNIFORM_BUFFER, sizeof(EllipsoidStruct) * 100, EllipsoidsTransform, GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glLineWidth(1.5f);
    i = 0;
    for (std::unique_ptr<Cuboid> const& cuboid : Cuboids)
    {
        if (cuboid->isWired)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (cuboid->isDrawn)
            cuboid->Draw(ResourceManager::GetShader("shader").Use());
        CuboidStruct temp = { .position = glm::vec4(cuboid->transform.Position, 1.0f)
            , .size = glm::vec4(cuboid->transform.Size, 1.0f) };
        CuboidsTransform[i] = temp;
        i++;
    }
    int numCuboids = i;
    ResourceManager::GetShader("raymarching").Use();
    ResourceManager::GetShader("raymarching").SetInteger("u_iCurrentCuboidsNum", Cuboids.size());
    glBindBuffer(GL_UNIFORM_BUFFER, uboCuboids);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CuboidStruct) * 100, CuboidsTransform, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    

    for (std::unique_ptr<CGismosLine> const& gismosLine : GismosLines)
    {
        if (gismosLine->isWired)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (gismosLine->isDrawn)
            gismosLine->Draw(ResourceManager::GetShader("shader").Use());
    }

    for (std::unique_ptr<Cuboid> const& gismosMark : GismosMarks)
    {
        if (gismosMark->isWired)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (gismosMark->isDrawn)
            gismosMark->Draw(ResourceManager::GetShader("shader").Use());
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ScreenQuad->Draw(ResourceManager::GetShader("raymarching").Use(), FBO);
    glm::quat cameraQuaternion = glm::vec3(0, 0, 0);
    cameraQuaternion = glm::rotate(cameraQuaternion, glm::radians(cameraPitch), glm::vec3(0, 1, 0));
    cameraQuaternion = glm::rotate(cameraQuaternion, glm::radians(cameraYaw), glm::vec3(1, 0, 0));
    ResourceManager::GetShader("raymarching").SetVector4f("uCameraQuaternion"
        , cameraQuaternion.x
        , cameraQuaternion.y
        , cameraQuaternion.z
        , cameraQuaternion.w);
    ResourceManager::GetShader("raymarching").SetMatrix4("uView", view);
    ResourceManager::GetShader("raymarching").SetVector3f("uCameraPosition", cameraPos);
    ResourceManager::GetShader("raymarching").SetVector3f("uCameraFront", cameraFront);
}

void Scene::UI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("My name is window, ImGUI window");
    // Text that appears in the window
    ImGui::Text("Hello there adventurer!");
    ImGui::Checkbox("Raymarching", &_raymarching);
    // Checkbox that appears in the window
    for (size_t i = 0; i != Cuboids.size(); ++i)
    {
        ImGui::Text("Cuboid: ");
        std::string idSuffix = (std::string("##cuboid") + std::to_string(i)).c_str();
        ImGui::Checkbox(std::format("Draw{0}", idSuffix).c_str(), &Cuboids[i]->isDrawn);
        ImGui::SliderFloat(std::format("position x{0}", idSuffix).c_str(), &Cuboids[i]->transform.Position.x, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("position y{0}", idSuffix).c_str(), &Cuboids[i]->transform.Position.y, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("position z{0}", idSuffix).c_str(), &Cuboids[i]->transform.Position.z, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("size x{0}", idSuffix).c_str(), &Cuboids[i]->transform.Size.x, 0.0f, 20.0f);
        ImGui::SliderFloat(std::format("size y{0}", idSuffix).c_str(), &Cuboids[i]->transform.Size.y, 0.0f, 20.0f);
        ImGui::SliderFloat(std::format("size z{0}", idSuffix).c_str(), &Cuboids[i]->transform.Size.z, 0.0f, 20.0f);
    }
    // Slider that appears in the window
    for (size_t i = 0; i != Ellipsoids.size(); ++i)
    {
        ImGui::Text("Ellipsoid: ");
        std::string idSuffix = (std::string("##ellipsoid") + std::to_string(i)).c_str();
        ImGui::Checkbox(std::format("Draw{0}", idSuffix).c_str(), &Ellipsoids[i]->isDrawn);
        ImGui::SliderFloat(std::format("position x{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Position.x, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("position y{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Position.y, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("position z{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Position.z, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("size x{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Size.x, 0.0f, 20.0f);
        ImGui::SliderFloat(std::format("size y{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Size.y, 0.0f, 20.0f);
        ImGui::SliderFloat(std::format("size z{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Size.z, 0.0f, 20.0f);
    }
    //// Fancy color editor that appears in the window
    //ImGui::ColorEdit4("Color", color);
    // Ends the window
    ImGui::End();
    // Renders the ImGUI elements
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene::Dispose()
{
}
