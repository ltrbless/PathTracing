#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Global.h"
#include "Texture.h"

class Material{

public:
    
    std::string name;
    vec3d Kd; // diffuse
    vec3d Ks; // specular
    vec3d Ke; // emissive
    double Ns; 
    double Ni;
    Texture mp_kd;







};

#endif //