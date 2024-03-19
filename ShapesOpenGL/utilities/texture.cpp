#include <iostream>

#include "texture.h"
#include "glm/glm.hpp"


Texture2D::Texture2D()
    : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->Width = width;
    this->Height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    std::vector<unsigned char> colorData(width * height * 4); // 4 components: RGBA

    // Get the color data from the texture
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorData.data());

    // Access the color data as a 2D vector of glm::vec4
    // std::vector<std::vector<glm::vec4>> colorVector(height, std::vector<glm::vec4>(width));

    //std::cout << "Mask: " << std::endl;
    //for (int y = 0; y < height; y++) {
    //    for (int x = 0; x < width; x++) {
    //        int index = (y * width + x) * 4;
    //        if (colorData[index + 3])
    //            this->Mask.push_back(true);
    //        else
    //            this->Mask.push_back(false);
    //        /// 255.0f << " ";
    //        //glm::vec4 colorValue(
    //        //    colorData[index] / 255.0f,
    //        //    colorData[index + 1] / 255.0f,
    //        //    colorData[index + 2] / 255.0f,
    //        //    colorData[index + 3] / 255.0f
    //        //);
    //        //colorVector[y][x] = colorValue;
    //    }
    //    std::cout << std::endl;
    //}
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}