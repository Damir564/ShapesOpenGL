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


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


Scene::Scene(unsigned int width, unsigned int height) : Width(width), Height(height), Keys(), KeysProcessed(), Cuboids() {}

void Scene::Init()
{
    ResourceManager::LoadShader("shaders/shader.vert", "shaders/shader.frag", nullptr, "shader");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
        static_cast<float>(this->Width) / static_cast<float>(this->Height), 
        0.1f, 100.0f);
    ResourceManager::GetShader("shader").Use();
    ResourceManager::GetShader("shader").SetMatrix4("projection", projection);

    Cuboids.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(0.0f, 0.0f, 0.0f) 
        , glm::vec3(1.0f, 1.0f, 1.0f)
        , glm::vec3(0.0f, 0.0f, 0.0f)
        , glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))));
    Cuboids.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(3.0f, 0.0f, 0.0f)
        , glm::vec3(2.0f, 2.0f, 2.0f)
        , glm::vec3(0.0f, 0.0f, 0.0f)
        , glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))));
    //Cuboids.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(0.0f, 0.0f, 2.0f)
    //    , glm::vec3(2.0f, 2.0f, 2.0f)
    //    , glm::vec3(0.0f, 0.0f, 0.0f)
    //    , glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))));
    //Cuboids.push_back(std::make_unique<Cuboid>(Cuboid(glm::vec3(2.0f, 0.0f, 2.0f)
    //    , glm::vec3(2.0f, 2.0f, 2.0f)
    //    , glm::vec3(0.0f, 0.0f, 0.0f)
    //    , glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))));

    //Ellipsoids.push_back(std::make_unique<Ellipsoid>(Ellipsoid(glm::vec3(1.0f, 0.0f, 1.0f)
    //    , glm::vec3(4.0f, 2.0f, 4.0f)
    //    , glm::vec3(0.0f, 0.0f, 0.0f)
    //    , glm::vec4(0.0f, 0.0f, 0.0f, 0.2f))));
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
        for (const auto& b : Cuboids)
        {
            if (a == b)
                continue;

            a->collider->ApplyTransform(a->transform);
            b->collider->ApplyTransform(b->transform);

            if (GJK(*a->collider, *b->collider))
                std::cout << "Yessss!" << '\n';
            else
                std::cout << "____________No:(" << '\n';
            //std::cout << "A: " << '\n';
            //for (const auto& vertex : a->collider->vertices)
            //{
            //    std::cout << "x = " << vertex.x << "; y = " << vertex.y << "; z = " << vertex.z << '\n';
            //}
            //std::cout << "B: " << '\n';
            //for (const auto& vertex : b->collider->vertices)
            //{
            //    std::cout << "x = " << vertex.x << "; y = " << vertex.y << "; z = " << vertex.z << '\n';
            //}
            //std::cout << '\n';
        }
}

void Scene::Render()
{
    ResourceManager::GetShader("shader").Use();
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    // glm::mat4 model = glm::mat4(1.0f);
    ResourceManager::GetShader("shader").SetMatrix4("view", view); // make sure to initialize matrix to identity matrix first
    // ResourceManager::GetShader("shader").SetMatrix4("model", model);
    //uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
    //    std::chrono::system_clock::now().time_since_epoch()
    //).count();
    // std::cout << ms << std::endl;
    ResourceManager::GetShader("shader").SetFloat("time", glfwGetTime());// / (float)sizeof(uint64_t) * 20000.0f);
    // std::cout << (float)(ms % 1000) / 1000.0f << std::endl;
    //if (style == glObject::STYLE::WIREFRAME) 
    
    // glPolygonMode(GL_FRONT, GL_LINE);
    //if (style == glObject::STYLE::SOLID)     
    // glPolygonMode(GL_FRONT, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //vaoVector[0].Bind();
    //glDrawElements(GL_TRIANGLES, (slices * stacks + slices) * 3 * 2, GL_UNSIGNED_INT, nullptr);
    //vaoVector[0].Unbind();

    //vaoVector[0].Bind();
    //glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_INT, nullptr);
    //vaoVector[0].Unbind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (std::unique_ptr<Ellipsoid> const& ellipsoid : Ellipsoids)
    {
        if (ellipsoid->isWired)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (ellipsoid->isDrawn)
            ellipsoid->Draw(ResourceManager::GetShader("shader").Use());       
    } 
    for (std::unique_ptr<Cuboid> const& cuboid : Cuboids)
    {
        if (cuboid->isWired)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (cuboid->isDrawn)
            cuboid->Draw(ResourceManager::GetShader("shader").Use());
        
    }
  

    

    
    // glBindVertexArray(0);
}

void Scene::UI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("My name is window, ImGUI window");
    // Text that appears in the window
    ImGui::Text("Hello there adventurer!");
    // Checkbox that appears in the window
    for (size_t i = 0; i != Cuboids.size(); ++i)
    {
        ImGui::Text("Cuboid: ");
        std::string idSuffix = (std::string("##cuboid") + std::to_string(i)).c_str();
        ImGui::Checkbox(std::format("Draw{0}", idSuffix).c_str(), &Cuboids[i]->isDrawn);
        ImGui::Checkbox(std::format("Wired{0}", idSuffix).c_str(), &Cuboids[i]->isWired);
        ImGui::SliderFloat(std::format("position x{0}", idSuffix).c_str(), &Cuboids[i]->transform.Position.x, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("position y{0}", idSuffix).c_str(), &Cuboids[i]->transform.Position.y, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("position z{0}", idSuffix).c_str(), &Cuboids[i]->transform.Position.z, -20.0f, 20.0f);
    }
    // Slider that appears in the window
    for (size_t i = 0; i != Ellipsoids.size(); ++i)
    {
        ImGui::Text("Ellipsoid: ");
        std::string idSuffix = (std::string("##ellipsoid") + std::to_string(i)).c_str();
        ImGui::Checkbox(std::format("Draw{0}", idSuffix).c_str(), &Ellipsoids[i]->isDrawn);
        ImGui::Checkbox(std::format("Wired{0}", idSuffix).c_str(), &Ellipsoids[i]->isWired);
        ImGui::SliderFloat(std::format("position x{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Position.x, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("position y{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Position.y, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("position z{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Position.z, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("size x{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Size.x, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("size y{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Size.y, -20.0f, 20.0f);
        ImGui::SliderFloat(std::format("size z{0}", idSuffix).c_str(), &Ellipsoids[i]->transform.Size.z, -20.0f, 20.0f);
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
