#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

bool Texture::LoadTexture(std::string filename)
{
    this->filename = filename;
    this->data = stbi_load(filename.c_str(), &this->width, &this->height, &this->channels, 0);
    if(this->data == nullptr)
    {
        std::cout << "Error: Could not load texture : " << filename << std::endl;
        return false;
    }
    return true;
}