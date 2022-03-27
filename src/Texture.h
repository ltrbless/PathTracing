#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <iostream>
#include "Global.h"
// #include "stb_image.h"

class Texture{

public:


    std::string filename;
    int width;
    int height;
    int channels; // [1 - 4]
    unsigned char *data; // left up to right down
    Texture() : width(0), height(0), channels(0), data(nullptr) {};

    bool LoadTexture(std::string filename);


};


#endif //