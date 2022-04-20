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
    else
    {
        std::cout << "Success load texture : " << filename << " width is : " << width << "   height is : " << height <<  "\n"; 
    }
    return true;
}

// Reference : Bilinear Interpolation
vec3d Texture::GetTexture(vec2d uv)
{
    if(uv.x() > 1.0) uv.x() -= int(uv.x());
    if(uv.y() > 1.0) uv.y() -= int(uv.y());

    double x = uv.x() * this->width;
    double y = uv.y() * this->height;


    int x_min = std::max(0, (int)x);
    int x_max = std::min(this->width - 1, int(x + 0.5));

    int y_min = std::max(0, int(y));
    int y_max = std::min(this->height - 1, int(y + 0.5));

    vec3d u00, u10, u01, u11;
    double R, G, B;
    
    //  ------------------ X
    //  |
    //  |   u00 --- u01
    //  |    |       |
    //  |    |       |
    //  Y   u10 --- u11

    R = (int)this->data[ this->channels * (this->width * y_min + x_min) + 0 ] / 255.0;
    G = (int)this->data[ this->channels * (this->width * y_min + x_min) + 1 ] / 255.0;
    B = (int)this->data[ this->channels * (this->width * y_min + x_min) + 2 ] / 255.0;
    u00 = vec3d(R, G, B);

    R = (int)this->data[ this->channels * (this->width * y_min + x_max) + 0 ] / 255.0;
    G = (int)this->data[ this->channels * (this->width * y_min + x_max) + 1 ] / 255.0;
    B = (int)this->data[ this->channels * (this->width * y_min + x_max) + 2 ] / 255.0;
    u01 = vec3d(R, G, B);

    R = (int)this->data[ this->channels * (this->width * y_max + x_min) + 0 ] / 255.0;
    G = (int)this->data[ this->channels * (this->width * y_max + x_min) + 1 ] / 255.0;
    B = (int)this->data[ this->channels * (this->width * y_max + x_min) + 2 ] / 255.0;
    u10 = vec3d(R, G, B);
    
    R = (int)this->data[ this->channels * (this->width * y_max + x_max) + 0 ] / 255.0;
    G = (int)this->data[ this->channels * (this->width * y_max + x_max) + 1 ] / 255.0;
    B = (int)this->data[ this->channels * (this->width * y_max + x_max) + 2 ] / 255.0;
    u11 = vec3d(R, G, B);

    double s = x - x_min;
    double t = y - y_min;

    vec3d u_mid1 = u00 + s * (u01 - u00);
    vec3d u_mid2 = u10 + s * (u11 - u10);

    return vec3d(u_mid1 + t * (u_mid2 - u_mid1));
}