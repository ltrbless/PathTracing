#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool Texture::LoadTexture(std::string filename)
{
    this->data = stbi_load(filename.c_str(), &this->width, &this->height, &this->channels, 0);
    if(this->data == nullptr)
    {
        std::cout << "Error: Could not load texture : " << filename << std::endl;
        return false;
    }
    return true;
}